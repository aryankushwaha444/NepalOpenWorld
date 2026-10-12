#include "SaveSystem/SaveManagerSubsystem.h"
#include "SaveSystem/NOWSaveGame.h"
#include "Security/SaveValidation.h"
#include "Security/InputValidation.h"
#include "Economy/WalletSubsystem.h"
#include "Missions/MissionSubsystem.h"
#include "Police/WantedSubsystem.h"
#include "World/TimeOfDaySubsystem.h"
#include "Property/PropertySubsystem.h"
#include "Player/NOWCharacter.h"
#include "Kismet/GameplayStatics.h"

bool USaveManagerSubsystem::SaveGame(const FString &Slot)
{
    if (!UInputValidation::IsValidSaveSlotName(Slot))
        return false; // path-traversal guard
    UNOWSaveGame *S = NewObject<UNOWSaveGame>();
    if (ANOWCharacter *C = Cast<ANOWCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld())))
    {
        S->PlayerLocation = C->GetActorLocation();
        S->PlayerRotation = C->GetActorRotation();
        S->Health = C->GetHealthPercent() * 100.f;
        if (UInventoryComponent *Inv = C->FindComponentByClass<UInventoryComponent>())
            S->Inventory = Inv->GetAll();
        if (UCharacterCustomizationComponent *CC = C->FindComponentByClass<UCharacterCustomizationComponent>())
            S->Customization = CC->GetData();
    }
    if (UWalletSubsystem *W = GetGameInstance()->GetSubsystem<UWalletSubsystem>())
        W->GetLedger()->FillSave(S->WalletNPR, S->BankNPR, S->Transactions, S->CompletedTxIds);
    if (UMissionSubsystem *M = GetGameInstance()->GetSubsystem<UMissionSubsystem>())
        M->FillSave(S->Missions);
    if (UWantedSubsystem *P = GetGameInstance()->GetSubsystem<UWantedSubsystem>())
        S->WantedLevel = P->GetWantedLevel();
    if (UTimeOfDaySubsystem *T = GetGameInstance()->GetSubsystem<UTimeOfDaySubsystem>())
        S->TimeHour = T->GetHour();
    if (UPropertySubsystem *PR = GetGameInstance()->GetSubsystem<UPropertySubsystem>())
        S->OwnedPropertyIds = PR->GetOwnedIds();

    // Backup LAST GOOD save before overwriting (never lose progress to a bad write)
    if (HasSave(Slot))
        if (USaveGame *Old = UGameplayStatics::LoadGameFromSlot(Slot, 0))
            UGameplayStatics::SaveGameToSlot(Old, Slot + TEXT("_backup"), 0);
    return UGameplayStatics::SaveGameToSlot(S, Slot, 0);
}

bool USaveManagerSubsystem::LoadGame(const FString &Slot)
{
    if (!UInputValidation::IsValidSaveSlotName(Slot))
        return false;
    auto TryLoad = [&](const FString &SName) -> UNOWSaveGame *
    {
        if (!UGameplayStatics::DoesSaveGameExist(SName, 0))
            return nullptr;
        UNOWSaveGame *S = Cast<UNOWSaveGame>(UGameplayStatics::LoadGameFromSlot(SName, 0));
        return (S && USaveValidation::ValidateAndSanitize(S)) ? S : nullptr; // validate BEFORE applying
    };
    UNOWSaveGame *S = TryLoad(Slot);
    if (!S)
        S = TryLoad(Slot + TEXT("_backup")); // recovery path
    if (!S)
        return false; // corrupted -> refuse, never silent reset

    if (ANOWCharacter *C = Cast<ANOWCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld())))
    {
        C->RespawnAt(S->PlayerLocation);
        if (UInventoryComponent *Inv = C->FindComponentByClass<UInventoryComponent>())
            Inv->ApplySave(S->Inventory);
    }
    if (UWalletSubsystem *W = GetGameInstance()->GetSubsystem<UWalletSubsystem>())
        W->GetLedger()->ApplySave(S->WalletNPR, S->BankNPR, S->Transactions, S->CompletedTxIds);
    if (UMissionSubsystem *M = GetGameInstance()->GetSubsystem<UMissionSubsystem>())
        M->ApplySave(S->Missions);
    if (UWantedSubsystem *P = GetGameInstance()->GetSubsystem<UWantedSubsystem>())
        P->SetWantedLevel(S->WantedLevel);
    if (UTimeOfDaySubsystem *T = GetGameInstance()->GetSubsystem<UTimeOfDaySubsystem>())
        T->SetHour(S->TimeHour);
    return true;
}

bool USaveManagerSubsystem::HasSave(const FString &Slot) const
{
    return UInputValidation::IsValidSaveSlotName(Slot) && UGameplayStatics::DoesSaveGameExist(Slot, 0);
}

void USaveManagerSubsystem::RequestAutosave() { SaveGame(TEXT("NepalAuto")); }
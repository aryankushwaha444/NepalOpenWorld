#include "Security/SaveValidation.h"
#include "SaveSystem/NOWSaveGame.h"
#include "Security/InputValidation.h"
#include "Security/EconomyValidation.h"
#include "SaveSystem/SaveMigration.h"

bool USaveValidation::ValidateAndSanitize(UNOWSaveGame *S)
{
    if (!S)
        return false;
    if (!NOWSaveMigration::MigrateIfNeeded(S))
        return false;

    S->Health = FMath::Clamp(S->Health, 0.f, 100.f);
    S->WalletNPR = UEconomyValidation::ClampBalance(S->WalletNPR);
    S->BankNPR = UEconomyValidation::ClampBalance(S->BankNPR);
    if (!UInputValidation::IsValidLocation(S->PlayerLocation))
        return false;
    S->WantedLevel = UInputValidation::SanitizeWantedLevel(S->WantedLevel);
    S->TimeHour = FMath::Clamp(S->TimeHour, 0.f, 24.f);

    // De-duplicate ownership arrays (blocks dup vehicle/property exploits)
    S->OwnedVehicleIds = TSet<FString>(S->OwnedVehicleIds).Array();
    S->OwnedPropertyIds = TSet<FString>(S->OwnedPropertyIds).Array();
    S->CompletedTxIds = TSet<FGuid>(S->CompletedTxIds).Array();

    for (const FTransactionRecord &R : S->Transactions)
        if (FMath::Abs(R.AmountNPR) > NOW_MAX_SINGLE_TRANSACTION)
            return false;
    for (auto &Pair : S->Inventory)
        Pair.Value = UInputValidation::SanitizeQuantity(Pair.Value);
    return true;
}
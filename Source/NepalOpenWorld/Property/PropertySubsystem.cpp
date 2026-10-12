#include "Property/PropertySubsystem.h"
#include "Economy/WalletSubsystem.h"
#include "Security/EconomyValidation.h"

void UPropertySubsystem::RegisterProperty(const FPropertyRecord &P)
{
    if (!P.PropertyId.IsEmpty())
        Props.Add(P.PropertyId, P);
}

bool UPropertySubsystem::BuyProperty(const FString &Id)
{
    FPropertyRecord *P = Props.Find(Id);
    if (!P || P->bOwned)
        return false; // duplicate ownership blocked BEFORE money moves
    if (!UEconomyValidation::IsValidAmount(P->PriceNPR))
        return false;
    auto *W = GetGameInstance()->GetSubsystem<UWalletSubsystem>();
    if (!W)
        return false;
    if (!W->Debit(FGuid::NewGuid(), P->PriceNPR, Id, TEXT("Purchase")))
        return false; // insufficient => fail safe
    P->bOwned = true;
    return true;
}

bool UPropertySubsystem::StoreVehicle(const FString &PropId, const FString &VehicleId)
{
    FPropertyRecord *P = Props.Find(PropId);
    if (!P || !P->bOwned || P->GarageVehicleIds.Contains(VehicleId))
        return false; // no duplicate garage entries
    if (P->GarageVehicleIds.Num() >= P->GarageCapacity)
        return false;
    P->GarageVehicleIds.Add(VehicleId);
    return true;
}

TArray<FString> UPropertySubsystem::GetOwnedIds() const
{
    TArray<FString> Out;
    for (const auto &Pair : Props)
        if (Pair.Value.bOwned)
            Out.Add(Pair.Key);
    return Out;
}
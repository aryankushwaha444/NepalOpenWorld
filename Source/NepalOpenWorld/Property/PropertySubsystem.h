#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PropertySubsystem.generated.h"

USTRUCT(BlueprintType)
struct FPropertyRecord
{
    GENERATED_BODY()
    UPROPERTY()
    FString PropertyId;
    UPROPERTY()
    int64 PriceNPR = 0;
    UPROPERTY()
    bool bOwned = false;
    UPROPERTY()
    int32 GarageCapacity = 2;
    UPROPERTY()
    TArray<FString> GarageVehicleIds;
};

UCLASS()
class UPropertySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void RegisterProperty(const FPropertyRecord &P);
    UFUNCTION(BlueprintCallable)
    bool BuyProperty(const FString &Id); // validated, no duplicate ownership
    UFUNCTION(BlueprintCallable)
    bool StoreVehicle(const FString &PropId, const FString &VehicleId);
    UFUNCTION(BlueprintCallable)
    TArray<FString> GetOwnedIds() const;

private:
    UPROPERTY()
    TMap<FString, FPropertyRecord> Props;
};
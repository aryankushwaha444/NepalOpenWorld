#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShopComponent.generated.h"

USTRUCT(BlueprintType)
struct FShopItem
{
    GENERATED_BODY()
    UPROPERTY()
    FString ItemId;
    UPROPERTY()
    int64 PriceNPR = 0;
    UPROPERTY()
    bool bUnique = false; // unique = cannot own twice
    UPROPERTY()
    bool bAvailable = true;
};

/** Shop logic NEVER trusts UI: full server-style validation chain before money moves. */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UShopComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void AddCatalogItem(const FShopItem &Item);
    UFUNCTION(BlueprintCallable)
    bool Buy(class APlayerController *PC, const FString &ItemId);

private:
    UPROPERTY()
    TMap<FString, FShopItem> Catalog;
    float LastBuyTime = -10.f; // input-spam guard
};
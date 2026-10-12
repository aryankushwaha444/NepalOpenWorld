#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    bool AddItem(const FString &Id, int32 Qty);
    UFUNCTION(BlueprintCallable)
    bool RemoveItem(const FString &Id, int32 Qty);
    UFUNCTION(BlueprintCallable)
    int32 GetCount(const FString &Id) const { return Items.Contains(Id) ? Items[Id] : 0; }
    UFUNCTION(BlueprintCallable)
    bool CanAccept(const FString &Id, int32 Qty) const;
    const TMap<FString, int32> &GetAll() const { return Items; }
    void ApplySave(const TMap<FString, int32> &In) { Items = In; }

private:
    UPROPERTY()
    TMap<FString, int32> Items;
    UPROPERTY()
    int32 MaxSlots = 50;
    UPROPERTY()
    int32 MaxStack = 99;
};
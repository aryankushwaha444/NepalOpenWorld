#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveManagerSubsystem.generated.h"

UCLASS()
class USaveManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    bool SaveGame(const FString &Slot);
    UFUNCTION(BlueprintCallable)
    bool LoadGame(const FString &Slot);
    UFUNCTION(BlueprintCallable)
    bool HasSave(const FString &Slot) const;
    UFUNCTION(BlueprintCallable)
    void RequestAutosave(); // call after missions/purchases/checkpoints
};
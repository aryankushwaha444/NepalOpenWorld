#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WantedSubsystem.generated.h"

UENUM(BlueprintType)
enum class ENOWCrimeSeverity : uint8
{
    Minor,
    Medium,
    Serious
};

UCLASS()
class UWantedSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    int32 GetWantedLevel() const { return WantedLevel; }
    UFUNCTION(BlueprintCallable)
    void SetWantedLevel(int32 L);
    UFUNCTION(BlueprintCallable)
    void RegisterCrime(ENOWCrimeSeverity Sev, bool bWitnessed);
    UFUNCTION(BlueprintCallable)
    bool IssueFine(int64 AmountNPR, const FString &Reason); // player PAYS; crimes never pay the player
private:
    UPROPERTY()
    int32 WantedLevel = 0;
    FTimerHandle DecayTimer;
    UFUNCTION()
    void Decay();
};
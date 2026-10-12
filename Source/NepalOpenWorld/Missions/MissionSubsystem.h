#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MissionSubsystem.generated.h"

UENUM(BlueprintType)
enum class ENOWMissionState : uint8
{
    Available,
    Active,
    Completed,
    Failed,
    Rewarded
};

USTRUCT(BlueprintType)
struct FMissionRecord
{
    GENERATED_BODY()
    UPROPERTY()
    FString MissionId;
    UPROPERTY()
    ENOWMissionState State = ENOWMissionState::Available;
    UPROPERTY()
    bool bRewardPaid = false;
    UPROPERTY()
    FGuid RewardEventId;
};

class UMissionDefinition;
class URewardService;

UCLASS()
class UMissionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase &Collection) override;
    UFUNCTION(BlueprintCallable)
    void RegisterDefinition(UMissionDefinition *Def);
    UFUNCTION(BlueprintCallable)
    bool StartMission(const FString &MissionId);
    UFUNCTION(BlueprintCallable)
    bool CompleteMission(const FString &MissionId); // pays ONCE
    UFUNCTION(BlueprintCallable)
    FString GetTrackedMission() const { return Tracked; }
    void FillSave(TArray<FMissionRecord> &Out) const;
    void ApplySave(const TArray<FMissionRecord> &In);

private:
    bool PrereqsMet(const FString &Id) const;
    UPROPERTY()
    TMap<FString, UMissionDefinition *> Defs;
    UPROPERTY()
    TMap<FString, FMissionRecord> Records;
    UPROPERTY()
    URewardService *Rewards = nullptr;
    UPROPERTY()
    FString Tracked;
};
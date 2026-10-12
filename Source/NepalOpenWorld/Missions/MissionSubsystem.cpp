#include "Missions/MissionSubsystem.h"
#include "Missions/MissionDefinition.h"
#include "Missions/RewardService.h"
#include "Economy/WalletSubsystem.h"

void UMissionSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
    Rewards = NewObject<URewardService>(this);
    if (UWalletSubsystem *W = GetGameInstance()->GetSubsystem<UWalletSubsystem>())
        Rewards->Init(W->GetLedger());
}

void UMissionSubsystem::RegisterDefinition(UMissionDefinition *Def)
{
    if (Def && !Def->MissionId.IsEmpty())
    {
        Defs.Add(Def->MissionId, Def);
        Records.FindOrAdd(Def->MissionId);
    }
}

bool UMissionSubsystem::PrereqsMet(const FString &Id) const
{
    const UMissionDefinition *D = Defs.Contains(Id) ? Defs[Id] : nullptr;
    if (!D)
        return false;
    for (const FString &P : D->PrerequisiteMissionIds)
    {
        const FMissionRecord *R = Records.Find(P);
        if (!R || (R->State != ENOWMissionState::Completed && R->State != ENOWMissionState::Rewarded))
            return false;
    }
    return true;
}

bool UMissionSubsystem::StartMission(const FString &MissionId)
{
    FMissionRecord *R = Records.Find(MissionId);
    if (!R || !PrereqsMet(MissionId))
        return false;
    if (R->State != ENOWMissionState::Available)
        return false;
    R->State = ENOWMissionState::Active;
    Tracked = MissionId;
    return true;
}

bool UMissionSubsystem::CompleteMission(const FString &MissionId)
{
    FMissionRecord *R = Records.Find(MissionId);
    if (!R || R->State != ENOWMissionState::Active || R->bRewardPaid)
        return false; // UI alone can never pay
    R->State = ENOWMissionState::Completed;
    const int64 Reward = Defs.Contains(MissionId) ? Defs[MissionId]->RewardNPR : 0;
    if (Reward > 0)
    {
        if (!R->RewardEventId.IsValid())
            R->RewardEventId = FGuid::NewGuid();
        if (!Rewards->GrantReward(R->RewardEventId, Reward, MissionId))
            return false; // economy refused
    }
    R->bRewardPaid = true;
    R->State = ENOWMissionState::Rewarded;
    if (USaveManagerSubsystemPlaceholder())
    {
    } // noop guard
    return true;
}

void UMissionSubsystem::FillSave(TArray<FMissionRecord> &Out) const { Out = Records.GenerateValueArray(); }
void UMissionSubsystem::ApplySave(const TArray<FMissionRecord> &In)
{
    for (const FMissionRecord &R : In)
        Records.Add(R.MissionId, R);
}
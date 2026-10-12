#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MissionDefinition.generated.h"

UCLASS(BlueprintType)
class UMissionDefinition : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    FString MissionId; // stable id, NOT display name
    UPROPERTY(EditAnywhere)
    FText Title; // localized via string tables later
    UPROPERTY(EditAnywhere)
    int64 RewardNPR = 0;
    UPROPERTY(EditAnywhere)
    TArray<FString> PrerequisiteMissionIds;
    UPROPERTY(EditAnywhere)
    bool bReplayable = false; // replayable payouts must be explicit + limited
};
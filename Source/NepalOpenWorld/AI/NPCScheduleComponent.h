#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCScheduleComponent.generated.h"

USTRUCT(BlueprintType)
struct FScheduleEntry
{
    GENERATED_BODY()
    UPROPERTY()
    int32 StartHour = 0;
    UPROPERTY()
    int32 EndHour = 24;
    UPROPERTY()
    FName Activity = TEXT("Idle"); // Work, School, Shop, Nightlife...
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNPCScheduleComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    FName GetActivity(float Hour) const;
    UFUNCTION(BlueprintCallable)
    void AddEntry(const FScheduleEntry &E);

private:
    UPROPERTY()
    TArray<FScheduleEntry> Entries;
};
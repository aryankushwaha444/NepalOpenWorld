#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TimeOfDaySubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHourChanged, float);

UCLASS()
class UTimeOfDaySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Tick(float DT) override;
    virtual bool IsTickable() const override { return true; }
    UFUNCTION(BlueprintCallable)
    float GetHour() const { return Hour; }
    UFUNCTION(BlueprintCallable)
    void SetHour(float H)
    {
        Hour = FMath::Clamp(H, 0.f, 24.f);
        OnHourChanged.Broadcast(Hour);
    }
    UFUNCTION(BlueprintCallable)
    FString GetTimeText() const;
    FOnHourChanged OnHourChanged; // level Blueprint rotates the sun from this single source of truth
private:
    UPROPERTY()
    float Hour = 8.f;
    UPROPERTY()
    float RealSecondsPerGameMinute = 1.f;
};
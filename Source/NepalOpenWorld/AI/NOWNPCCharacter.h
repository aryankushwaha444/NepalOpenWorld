#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NOWNPCCharacter.generated.h"

UCLASS()
class ANOWNPCCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    ANOWNPCCharacter();
    virtual void BeginPlay() override;
    UFUNCTION()
    void Wander();

protected:
    UPROPERTY()
    class UNPCScheduleComponent *Schedule;
    FTimerHandle WanderTimer;
};
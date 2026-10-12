#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NOWCharacter.generated.h"

UCLASS()
class ANOWCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    ANOWCharacter();
    UFUNCTION(BlueprintCallable)
    void SetFocusActor(AActor *A) { FocusActor = A; }
    UFUNCTION(BlueprintCallable)
    AActor *GetFocusActor() const { return FocusActor; }
    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const { return Health / MaxHealth; }
    UFUNCTION(BlueprintCallable)
    void ApplyValidatedDamage(float Amount, const FString &Source); // only validated damage enters here
    UFUNCTION(BlueprintCallable)
    void RespawnAt(const FVector &Loc);
    UFUNCTION(BlueprintCallable)
    void SetSprint(bool bOn);
    bool IsDead() const { return bIsDead; }

protected:
    UPROPERTY()
    float MaxHealth = 100.f;
    UPROPERTY()
    float Health = 100.f;
    UPROPERTY()
    TWeakObjectPtr<AActor> FocusActor;
    bool bIsDead = false;
    void Die();
};
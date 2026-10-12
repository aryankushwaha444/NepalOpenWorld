#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WildlifeCharacter.generated.h"

UENUM(BlueprintType)
enum class EHabitat : uint8
{
    Jungle,
    Mountain,
    River,
    Urban
};

UCLASS()
class AWildlifeCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AWildlifeCharacter();
    virtual void BeginPlay() override;
    virtual void Tick(float DT) override;
    UPROPERTY(EditAnywhere)
    EHabitat Habitat = EHabitat::Jungle; // spawner must match researched habitat
    UPROPERTY(EditAnywhere)
    bool bDangerous = false;
    UPROPERTY(EditAnywhere)
    float AttackDamage = 15.f;
    UPROPERTY(EditAnywhere)
    float AggroRadius = 300.f;

private:
    float AttackCooldown = 0.f;
    FTimerHandle WanderTimer;
    UFUNCTION()
    void Wander();
};
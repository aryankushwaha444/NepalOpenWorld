#include "AI/WildlifeCharacter.h"
#include "Player/NOWCharacter.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelper.h"
#include "Security/InputValidation.h"

AWildlifeCharacter::AWildlifeCharacter()
{
    AIControllerClass = AAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedAndSpawned;
    PrimaryActorTick.bCanEverTick = true;
}

void AWildlifeCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(WanderTimer, this, &AWildlifeCharacter::Wander, 6.f, true);
}

void AWildlifeCharacter::Wander()
{
    if (!GetController())
        return;
    const FVector T = GetActorLocation() + FVector(FMath::RandRange(-600.f, 600.f), FMath::RandRange(-600.f, 600.f), 0.f);
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), T);
}

void AWildlifeCharacter::Tick(float DT)
{
    Super::Tick(DT);
    AttackCooldown = FMath::Max(0.f, AttackCooldown - DT);
    if (!bDangerous || AttackCooldown > 0.f)
        return;
    if (APawn *P = UGameplayStatics::GetPlayerPawn(GetWorld()))
    {
        if (auto *C = Cast<ANOWCharacter>(P))
        {
            if (FVector::Dist(GetActorLocation(), C->GetActorLocation()) < AggroRadius)
            {
                C->ApplyValidatedDamage(AttackDamage, TEXT("Wildlife")); // validated path only
                AttackCooldown = 1.5f;
            }
        }
    }
}
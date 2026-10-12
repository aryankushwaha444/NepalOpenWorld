#include "AI/NOWNPCCharacter.h"
#include "AI/NPCScheduleComponent.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelper.h"
#include "NavigationSystem.h"

ANOWNPCCharacter::ANOWNPCCharacter()
{
    AIControllerClass = AAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedAndSpawned;
    Schedule = CreateDefaultSubobject<UNPCScheduleComponent>(TEXT("Schedule"));
}

void ANOWNPCCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(WanderTimer, this, &ANOWNPCCharacter::Wander, 5.f, true);
}

void ANOWNPCCharacter::Wander()
{
    if (!GetController())
        return;
    const FVector Origin = GetActorLocation();
    const FVector Target = Origin + FVector(FMath::RandRange(-800.f, 800.f), FMath::RandRange(-800.f, 800.f), 0.f);
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Target);
}
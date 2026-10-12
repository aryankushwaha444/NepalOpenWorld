#include "World/WorldInteractionComponent.h"
#include "Player/NOWCharacter.h"

UWorldInteractionComponent::UWorldInteractionComponent()
{
    InitSphereRadius(150.f);
    SetCollisionProfileName(TEXT("Trigger"));
    SetGenerateOverlapEvents(true);
}

void UWorldInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
    OnComponentBeginOverlap.AddDynamic(this, &UWorldInteractionComponent::OvBegin);
    OnComponentEndOverlap.AddDynamic(this, &UWorldInteractionComponent::OvEnd);
}

void UWorldInteractionComponent::OvBegin(UPrimitiveComponent *, AActor *Other, UPrimitiveComponent *, int32, bool, const FHitResult &)
{
    if (auto *C = Cast<ANOWCharacter>(Other))
        C->SetFocusActor(GetOwner());
}

void UWorldInteractionComponent::OvEnd(UPrimitiveComponent *, AActor *Other, UPrimitiveComponent *, int32)
{
    if (auto *C = Cast<ANOWCharacter>(Other))
        if (C->GetFocusActor() == GetOwner())
            C->SetFocusActor(nullptr);
}
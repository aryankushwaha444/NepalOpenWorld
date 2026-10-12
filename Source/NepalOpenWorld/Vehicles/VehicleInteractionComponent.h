#pragma once
#include "World/WorldInteractionComponent.h"
#include "VehicleInteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UVehicleInteractionComponent : public UWorldInteractionComponent
{
    GENERATED_BODY()
public:
    UVehicleInteractionComponent()
    {
        InitSphereRadius(250.f);
        PromptText = TEXT("Press E");
    }
};
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NOWGameMode.generated.h"

UCLASS()
class ANOWGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ANOWGameMode();
    virtual AActor *ChoosePlayerStart_Implementation(AController *NewPlayer) override; // Janaki Temple rule
};
#include "Core/NOWGameMode.h"
#include "Core/NOWPlayerController.h"
#include "Player/NOWCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

ANOWGameMode::ANOWGameMode()
{
    DefaultPawnClass = ANOWCharacter::StaticClass();
    PlayerControllerClass = ANOWPlayerController::StaticClass();
}

AActor *ANOWGameMode::ChoosePlayerStart_Implementation(AController *NewPlayer)
{
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
        if (It->Tags.Contains(FName("JanakiTempleStart")))
            return *It; // GDD rule: always start here
    return Super::ChoosePlayerStart_Implementation(NewPlayer);
}
#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WeatherSubsystem.generated.h"

UENUM(BlueprintType)
enum class EWeatherState : uint8
{
    Clear,
    Cloudy,
    Rain,
    HeavyRain,
    Fog
};
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeatherChanged, EWeatherState);

/** ONE authoritative weather state: visuals, road physics, NPC and audio all read from here (consistency rule). */
UCLASS()
class UWeatherSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    EWeatherState GetWeather() const { return Current; }
    UFUNCTION(BlueprintCallable)
    void SetWeather(EWeatherState W);
    UFUNCTION(BlueprintCallable)
    bool IsWetRoad() const { return Current == EWeatherState::Rain || Current == EWeatherState::HeavyRain; }
    FOnWeatherChanged OnWeatherChanged;

private:
    UPROPERTY()
    EWeatherState Current = EWeatherState::Clear;
};
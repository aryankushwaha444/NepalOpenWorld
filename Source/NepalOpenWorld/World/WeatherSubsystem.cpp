#include "World/WeatherSubsystem.h"

void UWeatherSubsystem::SetWeather(EWeatherState W)
{
    if (Current == W)
        return;
    Current = W;
    OnWeatherChanged.Broadcast(Current); // level BP swaps materials/fog/audio together => no desync
}
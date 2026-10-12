#include "World/TimeOfDaySubsystem.h"

void UTimeOfDaySubsystem::Tick(float DT)
{
    Hour += (DT / RealSecondsPerGameMinute) / 60.f;
    if (Hour >= 24.f)
        Hour -= 24.f;
    OnHourChanged.Broadcast(Hour);
}

FString UTimeOfDaySubsystem::GetTimeText() const
{
    const int32 H = FMath::FloorToInt(Hour);
    const int32 M = FMath::FloorToInt((Hour - H) * 60);
    return FString::Printf(TEXT("%02d:%02d"), H, M);
}
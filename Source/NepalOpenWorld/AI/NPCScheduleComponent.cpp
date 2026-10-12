#include "AI/NPCScheduleComponent.h"

void UNPCScheduleComponent::AddEntry(const FScheduleEntry &E) { Entries.Add(E); }

FName UNPCScheduleComponent::GetActivity(float Hour) const
{
    const int32 H = FMath::Clamp(FMath::FloorToInt(Hour), 0, 23);
    for (const FScheduleEntry &E : Entries)
        if (H >= E.StartHour && H < E.EndHour)
            return E.Activity;
    return TEXT("Idle");
}
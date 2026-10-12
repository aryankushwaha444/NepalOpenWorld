#include "Player/CharacterCustomizationComponent.h"

void FCustomizationData::Validate()
{
    auto C = [](int32 V)
    { return FMath::Clamp(V, 0, 31); }; // 32 options per category max (config later)
    SkinTone = C(SkinTone);
    FaceShape = C(FaceShape);
    Hairstyle = C(Hairstyle);
    HairColor = C(HairColor);
    EyeShape = C(EyeShape);
    EyeColor = C(EyeColor);
    EyebrowStyle = C(EyebrowStyle);
    NoseShape = C(NoseShape);
    MouthShape = C(MouthShape);
    FacialHair = C(FacialHair);
}

void UCharacterCustomizationComponent::ApplyVisuals()
{
    // Prototype: log only. Real mesh/material swaps attach here without touching save logic.
    UE_LOG(LogTemp, Log, TEXT("Customization applied: avatar=%d hair=%d"), (int32)Data.Avatar, Data.Hairstyle);
}
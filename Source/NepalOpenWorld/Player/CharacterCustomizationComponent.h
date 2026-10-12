#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterCustomizationComponent.generated.h"

UENUM(BlueprintType)
enum class EAvatarType : uint8
{
    Male,
    Female
};

USTRUCT(BlueprintType)
struct FCustomizationData
{
    GENERATED_BODY()
    UPROPERTY()
    EAvatarType Avatar = EAvatarType::Male;
    UPROPERTY()
    int32 SkinTone = 0;
    UPROPERTY()
    int32 FaceShape = 0;
    UPROPERTY()
    int32 Hairstyle = 0;
    UPROPERTY()
    int32 HairColor = 0;
    UPROPERTY()
    int32 EyeShape = 0;
    UPROPERTY()
    int32 EyeColor = 0;
    UPROPERTY()
    int32 EyebrowStyle = 0;
    UPROPERTY()
    int32 NoseShape = 0;
    UPROPERTY()
    int32 MouthShape = 0;
    UPROPERTY()
    int32 FacialHair = 0;
    UPROPERTY()
    FString EquippedClothingId;
    void Validate(); // clamps every index so bad saves/UI can't break rigs or meshes
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UCharacterCustomizationComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void SetData(const FCustomizationData &In)
    {
        Data = In;
        Data.Validate();
        ApplyVisuals();
    }
    UFUNCTION(BlueprintCallable)
    const FCustomizationData &GetData() const { return Data; }

private:
    void ApplyVisuals(); // hook: swap meshes/materials per index (Blueprint implementable later)
    UPROPERTY()
    FCustomizationData Data;
};
#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputValidation.generated.h"

/** Central clamping/sanitizing for ALL untrusted values (UI, saves, events). */
UCLASS()
class UInputValidation : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    static float SanitizeDamage(float In) { return FMath::Clamp(In, 0.f, 1000.f); }
    UFUNCTION(BlueprintCallable)
    static int32 SanitizeQuantity(int32 In) { return FMath::Clamp(In, 0, 999); }
    UFUNCTION(BlueprintCallable)
    static int32 SanitizeWantedLevel(int32 In) { return FMath::Clamp(In, 0, 5); }
    UFUNCTION(BlueprintCallable)
    static bool IsValidLocation(const FVector &V)
    {
        return FMath::IsFinite(V.X) && FMath::IsFinite(V.Y) && FMath::IsFinite(V.Z) && FMath::Abs(V.X) < 1000000.f && FMath::Abs(V.Y) < 1000000.f && FMath::Abs(V.Z) < 100000.f;
    }
    /** Blocks path traversal / weird slot names like "../../x" */
    UFUNCTION(BlueprintCallable)
    static bool IsValidSaveSlotName(const FString &Name)
    {
        if (Name.Len() < 1 || Name.Len() > 64)
            return false;
        for (TCHAR C : Name)
            if (!FChar::IsAlnum(C) && C != '_' && C != '-')
                return false;
        return true;
    }
};
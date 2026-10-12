#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveValidation.generated.h"
class UNOWSaveGame;

UCLASS()
class USaveValidation : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    /** Returns false if save is unusable; otherwise repairs/clamps in place. */
    UFUNCTION(BlueprintCallable)
    static bool ValidateAndSanitize(UNOWSaveGame *Save);
};
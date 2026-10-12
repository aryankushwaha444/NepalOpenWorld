#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EconomyValidation.generated.h"

#define NOW_MAX_SINGLE_TRANSACTION 10000000LL // रु 1 crore per event cap
#define NOW_MAX_BALANCE 1000000000LL          // रु 1 arab total cap

UCLASS()
class UEconomyValidation : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    static bool IsValidAmount(int64 A) { return A > 0 && A <= NOW_MAX_SINGLE_TRANSACTION; }
    UFUNCTION(BlueprintCallable)
    static bool IsValidBalance(int64 B) { return B >= 0 && B <= NOW_MAX_BALANCE; }
    UFUNCTION(BlueprintCallable)
    static int64 ClampBalance(int64 B) { return FMath::Clamp(B, 0LL, NOW_MAX_BALANCE); }
};
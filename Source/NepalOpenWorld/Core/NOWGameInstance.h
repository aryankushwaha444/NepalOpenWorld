#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NOWGameInstance.generated.h"

UENUM(BlueprintType)
enum class ENOWLanguage : uint8
{
    Nepali,
    English,
    Hindi
};
UENUM(BlueprintType)
enum class ENOWSessionMode : uint8
{
    NewGame,
    Continue
};

UCLASS()
class UNOWGameInstance : public UGameInstance
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void SetLanguage(ENOWLanguage L) { Language = L; }
    UFUNCTION(BlueprintCallable)
    ENOWLanguage GetLanguage() const { return Language; }
    UFUNCTION(BlueprintCallable)
    void SetSessionMode(ENOWSessionMode M) { SessionMode = M; }
    UFUNCTION(BlueprintCallable)
    ENOWSessionMode GetSessionMode() const { return SessionMode; }

private:
    UPROPERTY()
    ENOWLanguage Language = ENOWLanguage::English;
    UPROPERTY()
    ENOWSessionMode SessionMode = ENOWSessionMode::NewGame;
};
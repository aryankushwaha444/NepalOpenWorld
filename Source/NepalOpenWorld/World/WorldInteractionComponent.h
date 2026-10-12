#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/SphereComponent.h"
#include "WorldInteractionComponent.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UNOWInteractable : public UInterface
{
    GENERATED_BODY()
};
class INOWInteractable
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    FString GetPrompt();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(AActor *Interactor);
};

/** Focus tracker: sets/clears the player's current interact target on overlap. */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UWorldInteractionComponent : public USphereComponent
{
    GENERATED_BODY()
public:
    UWorldInteractionComponent();
    UPROPERTY()
    FString PromptText = TEXT("Press E");

protected:
    virtual void BeginPlay() override;
    UFUNCTION()
    void OvBegin(UPrimitiveComponent *, AActor *Other, UPrimitiveComponent *, int32, bool, const FHitResult &);
    UFUNCTION()
    void OvEnd(UPrimitiveComponent *, AActor *Other, UPrimitiveComponent *, int32);
};
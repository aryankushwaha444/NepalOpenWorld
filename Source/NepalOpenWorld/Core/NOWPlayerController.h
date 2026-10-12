#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NOWPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UUserWidget;

UCLASS()
class ANOWPlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    ANOWPlayerController();
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable)
    FString GetWalletText() const;
    UFUNCTION(BlueprintCallable)
    void SaveGameNow();
    UFUNCTION(BlueprintCallable)
    void LoadGameNow();
    UFUNCTION(BlueprintCallable)
    void RespawnOption(int32 Option); // -1 show death UI, 0 temple, 1 safehouse, 2 checkpoint

#if !UE_BUILD_SHIPPING // DEBUG-ONLY: excluded from release builds (security rule)
    UFUNCTION(BlueprintCallable)
    void DebugStartMission(const FString &Id);
    UFUNCTION(BlueprintCallable)
    void DebugCompleteMission(const FString &Id);
#endif

protected:
    UPROPERTY()
    UInputMappingContext *DefaultIMC;
    UPROPERTY()
    UInputAction *IA_Move, *IA_Look, *IA_Jump, *IA_Sprint, *IA_Interact, *IA_Save, *IA_Load;
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> HUDWidgetClass;
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> DeathWidgetClass;

    void OnMove(const FInputActionValue &V);
    void OnLook(const FInputActionValue &V);
    void OnJump(const FInputActionValue &V);
    void OnSprintStart(const FInputActionValue &V);
    void OnSprintEnd(const FInputActionValue &V);
    void OnInteract(const FInputActionValue &V);
    void OnSave(const FInputActionValue &V);
    void OnLoad(const FInputActionValue &V);
    void TryInteract();
};
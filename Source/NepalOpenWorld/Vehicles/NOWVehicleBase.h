#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "World/WorldInteractionComponent.h"
#include "NOWVehicleBase.generated.h"

UENUM(BlueprintType)
enum class ENOWVehicleState : uint8
{
    Parked,
    BeingEntered,
    PlayerDriven,
    NPCDriven,
    BeingExited,
    Disabled
};
UENUM(BlueprintType)
enum class ENOWOwnership : uint8
{
    None,
    PlayerOwned,
    TemporarilyOccupied,
    Stolen,
    NPCOwned,
    GarageStored,
    Destroyed
};

/** Validated state machine: exactly one driver, no teleport-into-seat, no stuck exits. */
UCLASS()
class ANOWVehicleBase : public APawn, public INOWInteractable
{
    GENERATED_BODY()
public:
    ANOWVehicleBase();
    virtual void Tick(float DT) override;
    virtual FString GetPrompt_Implementation() override;
    virtual void Interact_Implementation(AActor *Interactor) override;

    bool TryEnter(APlayerController *PC);
    bool TryExit(APlayerController *PC);
    void SetDriverInput(float Throttle, float Steer)
    {
        ThrottleIn = Throttle;
        SteerIn = Steer;
    }
    UFUNCTION(BlueprintCallable)
    APawn *GetStoredRiderPawn() const { return StoredRider.Get(); }
    UFUNCTION(BlueprintCallable)
    ENOWOwnership GetOwnership() const { return Ownership; }
    UFUNCTION(BlueprintCallable)
    void SetOwnership(ENOWOwnership O) { Ownership = O; } // purchase system only
    UFUNCTION(BlueprintCallable)
    FString GetVehicleId() const { return VehicleId; }
    UFUNCTION(BlueprintCallable)
    void SetVehicleId(const FString &Id) { VehicleId = Id; }

protected:
    virtual void BeginPlay() override;
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent *Mesh;
    UPROPERTY(VisibleAnywhere)
    USpringArmComponent *SpringArm;
    UPROPERTY(VisibleAnywhere)
    UCameraComponent *Camera;
    UPROPERTY(VisibleAnywhere)
    class UVehicleInteractionComponent *Interaction;
    UPROPERTY()
    ENOWVehicleState State = ENOWVehicleState::Parked;
    UPROPERTY()
    ENOWOwnership Ownership = ENOWOwnership::None;
    UPROPERTY()
    FString VehicleId;
    UPROPERTY()
    TWeakObjectPtr<AController> Driver;
    UPROPERTY()
    TWeakObjectPtr<APawn> StoredRider;
    float Speed = 0.f, Yaw = 0.f, GroundZ = 0.f, ThrottleIn = 0.f, SteerIn = 0.f;
    UPROPERTY(EditAnywhere)
    float MaxSpeed = 1200.f,
          MaxReverse = 300.f, Accel = 800.f, SteerRate = 90.f, Drag = 0.8f;
};
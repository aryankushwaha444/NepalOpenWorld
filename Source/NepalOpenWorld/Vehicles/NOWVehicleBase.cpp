#include "Vehicles/NOWVehicleBase.h"
#include "Vehicles/VehicleInteractionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

ANOWVehicleBase::ANOWVehicleBase()
{
    PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);
    Mesh->SetCollisionProfileName(TEXT("BlockAll"));
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(Mesh);
    SpringArm->TargetArmLength = 500.f;
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Interaction = CreateDefaultSubobject<UVehicleInteractionComponent>(TEXT("Interaction"));
    Interaction->SetupAttachment(Mesh);
}

void ANOWVehicleBase::BeginPlay()
{
    Super::BeginPlay();
    GroundZ = GetActorLocation().Z;
    Yaw = GetActorRotation().Yaw;
}

FString ANOWVehicleBase::GetPrompt_Implementation()
{
    if (State == ENOWVehicleState::Parked)
        return TEXT("Press E to enter vehicle");
    if (State == ENOWVehicleState::PlayerDriven)
        return TEXT("Press E to exit vehicle");
    return TEXT("");
}

void ANOWVehicleBase::Interact_Implementation(AActor *Interactor)
{
    if (!Interactor)
        return;
    APlayerController *PC = Cast<APlayerController>(Interactor->GetController());
    if (!PC)
        return;
    if (State == ENOWVehicleState::Parked)
        TryEnter(PC);
    else if (State == ENOWVehicleState::PlayerDriven && Driver == PC)
        TryExit(PC);
}

bool ANOWVehicleBase::TryEnter(APlayerController *PC)
{
    if (!PC || State != ENOWVehicleState::Parked || Driver.IsValid())
        return false; // one-driver rule
    State = ENOWVehicleState::BeingEntered;
    APawn *Old = PC->GetPawn();
    PC->Possess(this);
    Driver = PC;
    State = ENOWVehicleState::PlayerDriven;
    if (Old && Old != this)
    {
        StoredRider = Old;
        Old->SetActorHiddenInGame(true);
        Old->SetActorEnableCollision(false);
        Old->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
    }
    if (Ownership == ENOWOwnership::None)
        Ownership = ENOWOwnership::TemporarilyOccupied; // theft != ownership
    return true;
}

bool ANOWVehicleBase::TryExit(APlayerController *PC)
{
    if (!PC || State != ENOWVehicleState::PlayerDriven || Driver != PC)
        return false;
    FVector ExitLoc = GetActorLocation() + GetActorRightVector() * 150.f;
    ExitLoc.Z = GroundZ;
    if (GetWorld()->OverlapBlockingTestByChannel(ExitLoc, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(50.f)))
        return false; // never exit into geometry
    State = ENOWVehicleState::BeingExited;
    if (APawn *Rider = StoredRider.Get())
    {
        Rider->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Rider->SetActorLocation(ExitLoc);
        Rider->SetActorHiddenInGame(false);
        Rider->SetActorEnableCollision(true);
        PC->Possess(Rider);
    }
    StoredRider.Reset();
    Driver.Reset();
    Speed = ThrottleIn = SteerIn = 0.f;
    State = ENOWVehicleState::Parked;
    return true;
}

void ANOWVehicleBase::Tick(float DT)
{
    Super::Tick(DT);
    if (State != ENOWVehicleState::PlayerDriven && State != ENOWVehicleState::NPCDriven)
        return;
    Speed += ThrottleIn * Accel * DT;
    Speed -= Speed * Drag * DT;
    Speed = FMath::Clamp(Speed, -MaxReverse, MaxSpeed);
    if (FMath::Abs(Speed) > 5.f)
        Yaw += SteerIn * SteerRate * DT * FMath::Sign(Speed);
    FRotator R = GetActorRotation();
    R.Yaw = Yaw;
    SetActorRotation(R);
    FVector Loc = GetActorLocation();
    Loc.Z = GroundZ;
    Loc += GetActorForwardVector() * Speed * DT;
    SetActorLocation(Loc, true); // swept move: collisions respected
}
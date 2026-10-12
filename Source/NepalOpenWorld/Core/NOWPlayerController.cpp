#include "Core/NOWPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "Player/NOWCharacter.h"
#include "Vehicles/NOWVehicleBase.h"
#include "World/WorldInteractionComponent.h"
#include "Economy/WalletSubsystem.h"
#include "Missions/MissionSubsystem.h"
#include "SaveSystem/SaveManagerSubsystem.h"
#include "Core/NOWGameInstance.h"

static const FString MAIN_SLOT = TEXT("NepalMain");

ANOWPlayerController::ANOWPlayerController()
{
    ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("/Game/Gameplay/Data/IMC_Default.IMC_Default"));
    ConstructorHelpers::FObjectFinder<UInputAction> A1(TEXT("/Game/Gameplay/Data/IA_Move.IA_Move"));
    ConstructorHelpers::FObjectFinder<UInputAction> A2(TEXT("/Game/Gameplay/Data/IA_Look.IA_Look"));
    ConstructorHelpers::FObjectFinder<UInputAction> A3(TEXT("/Game/Gameplay/Data/IA_Jump.IA_Jump"));
    ConstructorHelpers::FObjectFinder<UInputAction> A4(TEXT("/Game/Gameplay/Data/IA_Sprint.IA_Sprint"));
    ConstructorHelpers::FObjectFinder<UInputAction> A5(TEXT("/Game/Gameplay/Data/IA_Interact.IA_Interact"));
    ConstructorHelpers::FObjectFinder<UInputAction> A6(TEXT("/Game/Gameplay/Data/IA_Save.IA_Save"));
    ConstructorHelpers::FObjectFinder<UInputAction> A7(TEXT("/Game/Gameplay/Data/IA_Load.IA_Load"));
    ConstructorHelpers::FObjectFinder<UUserWidget> HUD(TEXT("/Game/UI/HUD/WBP_MainHUD.WBP_MainHUD"));
    ConstructorHelpers::FObjectFinder<UUserWidget> Death(TEXT("/Game/UI/Menus/WBP_DeathScreen.WBP_DeathScreen"));
    DefaultIMC = IMC.Object;
    IA_Move = A1.Object;
    IA_Look = A2.Object;
    IA_Jump = A3.Object;
    IA_Sprint = A4.Object;
    IA_Interact = A5.Object;
    IA_Save = A6.Object;
    IA_Load = A7.Object;
    HUDWidgetClass = HUD.Object;
    DeathWidgetClass = Death.Object;
}

void ANOWPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (auto *Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        if (DefaultIMC)
            Sub->AddMappingContext(DefaultIMC, 0);
    if (HUDWidgetClass)
        if (UUserWidget *W = CreateWidget(this, HUDWidgetClass))
            W->AddToViewport();
}

void ANOWPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    auto *EIC = Cast<UEnhancedInputComponent>(InputComponent);
    if (!EIC)
        return;
    EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ANOWPlayerController::OnMove);
    EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ANOWPlayerController::OnLook);
    EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &ANOWPlayerController::OnJump);
    EIC->BindAction(IA_Sprint, ETriggerEvent::Started, this, &ANOWPlayerController::OnSprintStart);
    EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ANOWPlayerController::OnSprintEnd);
    EIC->BindAction(IA_Interact, ETriggerEvent::Started, this, &ANOWPlayerController::OnInteract);
    EIC->BindAction(IA_Save, ETriggerEvent::Started, this, &ANOWPlayerController::OnSave);
    EIC->BindAction(IA_Load, ETriggerEvent::Started, this, &ANOWPlayerController::OnLoad);
}

void ANOWPlayerController::OnMove(const FInputActionValue &V)
{
    APawn *P = GetPawn();
    if (!P)
        return;
    const FVector2D Ax = V.Get<FVector2D>();
    if (auto *C = Cast<ANOWCharacter>(P))
    {
        if (C->IsDead())
            return;
        const FRotator YR(0.f, GetControlRotation().Yaw, 0.f);
        C->AddMovementInput(FRotationMatrix(YR).GetUnitAxis(EAxis::X), Ax.Y);
        C->AddMovementInput(FRotationMatrix(YR).GetUnitAxis(EAxis::Y), Ax.X);
    }
    else if (auto *Veh = Cast<ANOWVehicleBase>(P))
        Veh->SetDriverInput(Ax.Y, Ax.X);
}

void ANOWPlayerController::OnLook(const FInputActionValue &V)
{
    if (Cast<ANOWCharacter>(GetPawn()))
    {
        const FVector2D Ax = V.Get<FVector2D>();
        AddControllerYawInput(Ax.X);
        AddControllerPitchInput(-Ax.Y);
    }
}

void ANOWPlayerController::OnJump(const FInputActionValue &)
{
    if (auto *C = Cast<ANOWCharacter>(GetPawn()))
        if (!C->IsDead())
            C->Jump();
}
void ANOWPlayerController::OnSprintStart(const FInputActionValue &)
{
    if (auto *C = Cast<ANOWCharacter>(GetPawn()))
        C->SetSprint(true);
}
void ANOWPlayerController::OnSprintEnd(const FInputActionValue &)
{
    if (auto *C = Cast<ANOWCharacter>(GetPawn()))
        C->SetSprint(false);
}
void ANOWPlayerController::OnInteract(const FInputActionValue &) { TryInteract(); }
void ANOWPlayerController::OnSave(const FInputActionValue &) { SaveGameNow(); }
void ANOWPlayerController::OnLoad(const FInputActionValue &) { LoadGameNow(); }

void ANOWPlayerController::TryInteract()
{
    if (auto *C = Cast<ANOWCharacter>(GetPawn()))
    {
        if (C->IsDead())
            return;
        if (AActor *F = C->GetFocusActor())
            if (F->GetClass()->ImplementsInterface(UNOWInteractable::StaticClass()))
                INOWInteractable::Execute_Interact(F, C);
    }
    else if (auto *Veh = Cast<ANOWVehicleBase>(GetPawn()))
        if (AActor *Rider = Veh->GetStoredRiderPawn())
            INOWInteractable::Execute_Interact(Veh, Rider); // exit path
}

FString ANOWPlayerController::GetWalletText() const
{
    if (const auto *W = GetGameInstance()->GetSubsystem<UWalletSubsystem>())
        return W->GetWalletText();
    return TEXT("रु 0");
}

void ANOWPlayerController::SaveGameNow()
{
    if (auto *SM = GetGameInstance()->GetSubsystem<USaveManagerSubsystem>())
    {
        SM->SaveGame(MAIN_SLOT);
        SM->RequestAutosave();
    }
}

void ANOWPlayerController::LoadGameNow()
{
    if (auto *SM = GetGameInstance()->GetSubsystem<USaveManagerSubsystem>())
        SM->LoadGame(MAIN_SLOT);
}

void ANOWPlayerController::RespawnOption(int32 Option)
{
    auto *C = Cast<ANOWCharacter>(GetPawn());
    if (!C)
        return;
    if (Option == -1)
    {
        if (DeathWidgetClass)
            if (UUserWidget *W = CreateWidget(this, DeathWidgetClass))
                W->AddToViewport();
        return;
    }
    FVector Spawn = FVector::ZeroVector; // safehouse/checkpoint resolved later; temple fallback always valid
    C->RespawnAt(Spawn);
}

#if !UE_BUILD_SHIPPING
void ANOWPlayerController::DebugStartMission(const FString &Id)
{
    if (auto *M = GetGameInstance()->GetSubsystem<UMissionSubsystem>())
        M->StartMission(Id);
}
void ANOWPlayerController::DebugCompleteMission(const FString &Id)
{
    if (auto *M = GetGameInstance()->GetSubsystem<UMissionSubsystem>())
        M->CompleteMission(Id);
}
#endif
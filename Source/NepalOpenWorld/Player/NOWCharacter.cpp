#include "Player/NOWCharacter.h"
#include "Core/NOWPlayerController.h"
#include "Security/InputValidation.h"

ANOWCharacter::ANOWCharacter() { GetCharacterMovement()->MaxWalkSpeed = 300.f; }

void ANOWCharacter::SetSprint(bool bOn) { GetCharacterMovement()->MaxWalkSpeed = bOn ? 600.f : 300.f; }

void ANOWCharacter::ApplyValidatedDamage(float Amount, const FString &Source)
{
    Amount = UInputValidation::SanitizeDamage(Amount); // clamp: malformed events can't create impossible health
    if (Amount <= 0.f || bIsDead)
        return;
    Health = FMath::Clamp(Health - Amount, 0.f, MaxHealth);
    if (Health <= 0.f)
        Die();
}

void ANOWCharacter::Die()
{
    bIsDead = true;
    GetCharacterMovement()->DisableMovement();
    if (auto *PC = Cast<ANOWPlayerController>(GetController()))
        PC->RespawnOption(-1);
}

void ANOWCharacter::RespawnAt(const FVector &Loc)
{
    if (!UInputValidation::IsValidLocation(Loc))
        return; // reject corrupted save positions
    SetActorLocation(Loc);
    Health = MaxHealth;
    bIsDead = false;
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
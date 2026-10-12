#include "Police/WantedSubsystem.h"
#include "Economy/WalletSubsystem.h"
#include "Security/InputValidation.h"
#include "Security/EconomyValidation.h"

void UWantedSubsystem::SetWantedLevel(int32 L)
{
    WantedLevel = UInputValidation::SanitizeWantedLevel(L);
    if (WantedLevel > 0 && !DecayTimer.IsValid())
        GetWorld()->GetTimerManager().SetTimer(DecayTimer, this, &UWantedSubsystem::Decay, 30.f, true);
}

void UWantedSubsystem::Decay()
{
    if (WantedLevel > 0)
        WantedLevel--;
    if (WantedLevel == 0)
        GetWorld()->GetTimerManager().ClearTimer(DecayTimer);
}

void UWantedSubsystem::RegisterCrime(ENOWCrimeSeverity Sev, bool bWitnessed)
{
    int32 Points = 0;
    switch (Sev)
    {
    case ENOWCrimeSeverity::Minor:
        Points = 1;
        break;
    case ENOWCrimeSeverity::Medium:
        Points = 2;
        break;
    default:
        Points = 3;
        break;
    }
    if (!bWitnessed && Sev != ENOWCrimeSeverity::Serious)
        Points = FMath::Max(0, Points - 1);
    if (Points > 0)
        SetWantedLevel(WantedLevel + Points); // consequences, never cash rewards
}

bool UWantedSubsystem::IssueFine(int64 AmountNPR, const FString &Reason)
{
    if (!UEconomyValidation::IsValidAmount(AmountNPR))
        return false;
    auto *W = GetGameInstance()->GetSubsystem<UWalletSubsystem>();
    return W && W->Debit(FGuid::NewGuid(), AmountNPR, Reason, TEXT("Fine"));
}
#include "UI/NOWHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Economy/WalletSubsystem.h"
#include "World/TimeOfDaySubsystem.h"
#include "Player/NOWCharacter.h"
#include "Kismet/GameplayStatics.h"

void UNOWHUD::NativeTick(const FGeometry &G, float DT)
{
    Super::NativeTick(G, DT);
    if (auto *W = GetGameInstance()->GetSubsystem<UWalletSubsystem>())
        WalletText->SetText(FText::FromString(W->GetWalletText()));
    if (auto *T = GetGameInstance()->GetSubsystem<UTimeOfDaySubsystem>())
        TimeText->SetText(FText::FromString(T->GetTimeText()));
    if (auto *C = Cast<ANOWCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld())))
        HealthBar->SetPercent(C->GetHealthPercent());
}
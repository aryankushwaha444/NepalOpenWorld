#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Missions/RewardService.h"
#include "Economy/TransactionLedger.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRewardPaidOnce, "NepalOpenWorld.Missions.RewardPaidOncePerEvent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FRewardPaidOnce::RunTest(const FString &)
{
    UTransactionLedger *L = NewObject<UTransactionLedger>();
    URewardService *R = NewObject<URewardService>();
    R->Init(L);
    const FGuid Event = FGuid::NewGuid();
    TestTrue("First payout", R->GrantReward(Event, 500, TEXT("M_Test")));
    TestFalse("Second payout same event blocked", R->GrantReward(Event, 500, TEXT("M_Test")));
    TestTrue("Balance correct", L->GetWallet() == 1500);
    TestFalse("Zero reward refused", R->GrantReward(FGuid::NewGuid(), 0, TEXT("M_Zero")));
    return true;
}
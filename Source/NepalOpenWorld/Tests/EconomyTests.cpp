#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Economy/TransactionLedger.h"
#include "Security/EconomyValidation.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEconomyDuplicateTx, "NepalOpenWorld.Economy.DuplicateTxBlocked", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FEconomyDuplicateTx::RunTest(const FString &)
{
    UTransactionLedger *L = NewObject<UTransactionLedger>();
    const FGuid Id = FGuid::NewGuid();
    TestTrue("First tx ok", L->Execute(Id, TEXT("Reward"), 500, TEXT("M1")));
    TestFalse("Replay blocked", L->Execute(Id, TEXT("Reward"), 500, TEXT("M1")));
    TestTrue("Wallet paid once", L->GetWallet() == 1500);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEconomyInsufficientFunds, "NepalOpenWorld.Economy.InsufficientFundsSafe", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
bool FEconomyInsufficientFunds::RunTest(const FString &)
{
    UTransactionLedger *L = NewObject<UTransactionLedger>();
    TestFalse("Overdraft refused", L->Execute(FGuid::NewGuid(), TEXT("Purchase"), -999999, TEXT("Car")));
    TestTrue("No deduction on failure", L->GetWallet() == 1000);
    TestFalse("Huge amount refused", L->Execute(FGuid::NewGuid(), TEXT("Reward"), NOW_MAX_SINGLE_TRANSACTION + 1, TEXT("X")));
    return true;
}
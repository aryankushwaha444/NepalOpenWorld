#include "Economy/WalletSubsystem.h"
#include "Security/EconomyValidation.h"

void UWalletSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
    Ledger = NewObject<UTransactionLedger>(this);
}

FString UWalletSubsystem::GetWalletText() const
{
    return Ledger ? FString::Printf(TEXT("रु %lld"), Ledger->GetWallet()) : TEXT("रु 0"); // NPR only, never $
}

bool UWalletSubsystem::Credit(const FGuid &TxId, int64 Amount, const FString &Ref, const FString &Type)
{
    if (!UEconomyValidation::IsValidAmount(Amount))
        return false; // rejects 0/negative/huge
    return Ledger && Ledger->Execute(TxId, Type, Amount, Ref);
}

bool UWalletSubsystem::Debit(const FGuid &TxId, int64 Amount, const FString &Ref, const FString &Type)
{
    if (!UEconomyValidation::IsValidAmount(Amount))
        return false;
    return Ledger && Ledger->Execute(TxId, Type, -Amount, Ref);
}

void UWalletSubsystem::RunConsistencyCheck() const
{
#if !UE_BUILD_SHIPPING
    if (!Ledger)
        return;
    int64 Sum = 0;
    for (const FTransactionRecord &R : Ledger->GetHistory())
        Sum += R.AmountNPR;
    if (Sum != Ledger->GetWallet() - 1000) // 1000 = prototype starting cash
        UE_LOG(LogTemp, Error, TEXT("ECONOMY INCONSISTENT: ledger sum %lld != wallet %lld"), Sum, Ledger->GetWallet());
#endif
}
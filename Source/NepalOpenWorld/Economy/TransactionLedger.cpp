#include "Economy/TransactionLedger.h"
#include "Security/EconomyValidation.h"

bool UTransactionLedger::Execute(const FGuid &TxId, const FString &Type, int64 SignedAmount, const FString &RelatedRef)
{
    if (!TxId.IsValid() || SignedAmount == 0)
        return false;
    if (CompletedTxIds.Contains(TxId))
        return false; // idempotency: replay blocked
    if (FMath::Abs(SignedAmount) > NOW_MAX_SINGLE_TRANSACTION)
        return false;
    if (SignedAmount < 0 && (Wallet + SignedAmount) < 0)
        return false; // insufficient funds: no deduction, nothing granted

    Wallet += SignedAmount;
    FTransactionRecord R;
    R.TxId = TxId;
    R.Type = Type;
    R.AmountNPR = SignedAmount;
    R.RelatedRef = RelatedRef;
    R.BalanceAfter = Wallet;
    History.Add(R);
    CompletedTxIds.Add(TxId);
    return true;
}

void UTransactionLedger::FillSave(int64 &W, int64 &B, TArray<FTransactionRecord> &H, TArray<FGuid> &I) const
{
    W = Wallet;
    B = Bank;
    H = History;
    I = CompletedTxIds.Array();
}

void UTransactionLedger::ApplySave(int64 W, int64 B, const TArray<FTransactionRecord> &H, const TArray<FGuid> &I)
{
    Wallet = UEconomyValidation::ClampBalance(W);
    Bank = UEconomyValidation::ClampBalance(B);
    History = H;
    CompletedTxIds.Empty();
    for (const FGuid &G : I)
        CompletedTxIds.Add(G);
}
#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TransactionLedger.generated.h"

USTRUCT(BlueprintType)
struct FTransactionRecord
{
    GENERATED_BODY()
    UPROPERTY()
    FGuid TxId;
    UPROPERTY()
    FString Type; // Credit | Debit | Reward | Purchase | Fine
    UPROPERTY()
    int64 AmountNPR = 0; // signed: + in, - out
    UPROPERTY()
    FString RelatedRef; // mission/item/vehicle/property id
    UPROPERTY()
    int64 BalanceAfter = 0;
};

/** Single authority for money math. Atomic + idempotent by transaction id. */
UCLASS()
class UTransactionLedger : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    int64 GetWallet() const { return Wallet; }
    UFUNCTION(BlueprintCallable)
    int64 GetBank() const { return Bank; }
    UFUNCTION(BlueprintCallable)
    const TArray<FTransactionRecord> &GetHistory() const { return History; }

    /** The ONLY way money moves. Returns false on duplicate/invalid/insufficient. */
    UFUNCTION(BlueprintCallable)
    bool Execute(const FGuid &TxId, const FString &Type, int64 SignedAmount, const FString &RelatedRef);

    void FillSave(int64 &OutWallet, int64 &OutBank, TArray<FTransactionRecord> &OutHistory, TArray<FGuid> &OutIds) const;
    void ApplySave(int64 InWallet, int64 InBank, const TArray<FTransactionRecord> &InHistory, const TArray<FGuid> &InIds);

private:
    UPROPERTY()
    int64 Wallet = 1000; // prototype starting cash, NPR only
    UPROPERTY()
    int64 Bank = 0;
    UPROPERTY()
    TArray<FTransactionRecord> History;
    UPROPERTY()
    TSet<FGuid> CompletedTxIds;
};
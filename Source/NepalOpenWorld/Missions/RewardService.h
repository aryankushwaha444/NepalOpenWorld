#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RewardService.generated.h"
class UTransactionLedger;

/** Maps a gameplay EVENT id to one permanent transaction id => replay/reload cannot pay twice. */
UCLASS()
class URewardService : public UObject
{
    GENERATED_BODY()
public:
    void Init(UTransactionLedger *InLedger) { Ledger = InLedger; }
    bool GrantReward(const FGuid &EventId, int64 AmountNPR, const FString &SourceId);

private:
    UPROPERTY()
    UTransactionLedger *Ledger = nullptr;
    UPROPERTY()
    TMap<FGuid, FGuid> EventToTx; // event -> tx, persisted via mission record
};
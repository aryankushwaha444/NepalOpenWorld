#include "Missions/RewardService.h"
#include "Economy/TransactionLedger.h"
#include "Security/EconomyValidation.h"

bool URewardService::GrantReward(const FGuid &EventId, int64 AmountNPR, const FString &SourceId)
{
    if (!EventId.IsValid() || !Ledger)
        return false;
    if (!UEconomyValidation::IsValidAmount(AmountNPR))
        return false; // no infinite-money values
    FGuid TxId;
    if (const FGuid *Found = EventToTx.Find(EventId))
        TxId = *Found; // same event => same tx => ledger blocks replay
    else
    {
        TxId = FGuid::NewGuid();
        EventToTx.Add(EventId, TxId);
    }
    return Ledger->Execute(TxId, TEXT("Reward"), AmountNPR, SourceId);
}
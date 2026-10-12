#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Economy/TransactionLedger.h"
#include "WalletSubsystem.generated.h"

UCLASS()
class UWalletSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase &Collection) override;

    UFUNCTION(BlueprintCallable)
    UTransactionLedger *GetLedger() const { return Ledger; }
    UFUNCTION(BlueprintCallable)
    FString GetWalletText() const;
    UFUNCTION(BlueprintCallable)
    bool Credit(const FGuid &TxId, int64 Amount, const FString &Ref, const FString &Type = TEXT("Credit"));
    UFUNCTION(BlueprintCallable)
    bool Debit(const FGuid &TxId, int64 Amount, const FString &Ref, const FString &Type = TEXT("Debit"));
    UFUNCTION(BlueprintCallable)
    void RunConsistencyCheck() const;

private:
    UPROPERTY()
    UTransactionLedger *Ledger = nullptr;
};
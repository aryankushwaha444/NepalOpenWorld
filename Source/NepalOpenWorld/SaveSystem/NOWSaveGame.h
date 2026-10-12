#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Economy/TransactionLedger.h"
#include "Missions/MissionSubsystem.h"
#include "Player/CharacterCustomizationComponent.h"
#include "NOWSaveGame.generated.h"

UCLASS()
class UNOWSaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY()
    int32 SaveVersion = NOW_SAVE_VERSION;
    UPROPERTY()
    FVector PlayerLocation = FVector::ZeroVector;
    UPROPERTY()
    FRotator PlayerRotation = FRotator::ZeroRotator;
    UPROPERTY()
    float Health = 100.f;
    UPROPERTY()
    uint8 Language = 1; // 0 Nepali, 1 English, 2 Hindi
    UPROPERTY()
    int32 RespawnPreference = 0;
    UPROPERTY()
    bool bIntroSeen = false;
    UPROPERTY()
    float TimeHour = 8.f;
    UPROPERTY()
    uint8 Weather = 0;
    UPROPERTY()
    int32 WantedLevel = 0;
    UPROPERTY()
    int64 WalletNPR = 1000;
    UPROPERTY()
    int64 BankNPR = 0;
    UPROPERTY()
    TArray<FTransactionRecord> Transactions;
    UPROPERTY()
    TArray<FGuid> CompletedTxIds;
    UPROPERTY()
    TArray<FMissionRecord> Missions;
    UPROPERTY()
    TArray<FString> OwnedVehicleIds;
    UPROPERTY()
    TArray<FString> OwnedPropertyIds;
    UPROPERTY()
    TMap<FString, int32> Inventory;
    UPROPERTY()
    FCustomizationData Customization;
};
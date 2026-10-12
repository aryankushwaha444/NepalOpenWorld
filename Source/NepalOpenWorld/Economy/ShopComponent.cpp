#include "Economy/ShopComponent.h"
#include "Economy/WalletSubsystem.h"
#include "Inventory/InventoryComponent.h"
#include "Security/EconomyValidation.h"
#include "GameFramework/PlayerController.h"

void UShopComponent::AddCatalogItem(const FShopItem &Item)
{
    if (Item.ItemId.IsEmpty())
        return;
    Catalog.Add(Item.ItemId, Item);
}

bool UShopComponent::Buy(APlayerController *PC, const FString &ItemId)
{
    if (!PC)
        return false;
    // 7) duplicate/rapid-request guard
    if (GetWorld()->GetTimeSeconds() - LastBuyTime < 0.25f)
        return false;
    LastBuyTime = GetWorld()->GetTimeSeconds();

    // 1) exists  2) available
    const FShopItem *Item = Catalog.Find(ItemId);
    if (!Item || !Item->bAvailable)
        return false;
    // 4) valid configured price
    if (!UEconomyValidation::IsValidAmount(Item->PriceNPR))
        return false;

    UInventoryComponent *Inv = PC->GetPawn() ? PC->GetPawn()->FindComponentByClass<UInventoryComponent>() : nullptr;
    if (!Inv)
        return false;
    // 6) capacity + unique ownership check
    if (Item->bUnique && Inv->GetCount(ItemId) > 0)
        return false;
    if (!Inv->CanAccept(ItemId, 1))
        return false;

    UWalletSubsystem *Wallet = GetGameInstance()->GetSubsystem<UWalletSubsystem>();
    if (!Wallet)
        return false;
    // 5) funds + atomic debit
    const FGuid TxId = FGuid::NewGuid();
    if (!Wallet->Debit(TxId, Item->PriceNPR, ItemId, TEXT("Purchase")))
        return false; // failed = nothing granted
    // 8) grant exactly once, only after payment succeeded
    Inv->AddItem(ItemId, 1);
    return true;
}
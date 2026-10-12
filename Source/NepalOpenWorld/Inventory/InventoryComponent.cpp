#include "Inventory/InventoryComponent.h"
#include "Security/InputValidation.h"

bool UInventoryComponent::CanAccept(const FString &Id, int32 Qty) const
{
    if (Id.IsEmpty() || Qty <= 0)
        return false;
    const int32 Cur = GetCount(Id);
    if (Cur == 0 && Items.Num() >= MaxSlots)
        return false;
    return (Cur + UInputValidation::SanitizeQuantity(Qty)) <= MaxStack;
}

bool UInventoryComponent::AddItem(const FString &Id, int32 Qty)
{
    Qty = UInputValidation::SanitizeQuantity(Qty);
    if (!CanAccept(Id, Qty))
        return false; // blocks duplication overflow
    Items.Add(Id, GetCount(Id) + Qty);
    return true;
}

bool UInventoryComponent::RemoveItem(const FString &Id, int32 Qty)
{
    Qty = UInputValidation::SanitizeQuantity(Qty);
    if (GetCount(Id) < Qty)
        return false; // blocks negative inventory / double-sell
    const int32 Left = GetCount(Id) - Qty;
    if (Left <= 0)
        Items.Remove(Id);
    else
        Items[Id] = Left;
    return true;
}
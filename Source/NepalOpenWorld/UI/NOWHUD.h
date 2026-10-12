#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NOWHUD.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class UNOWHUD : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock *WalletText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock *TimeText;
    UPROPERTY(meta = (BindWidget))
    UProgressBar *HealthBar;
};
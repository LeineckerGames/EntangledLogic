#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlRowWidget.generated.h"

class UInputAction;
class UInputKeyBindingSlot;
class UTextBlock;

UCLASS()
class ENTANGLEDLOGIC_API UControlRowWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void InitializeRow(FText ActionName, UInputAction* InAction, FKey PrimaryKey, FKey SecondaryKey);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ActionLabelText;

    UPROPERTY(meta = (BindWidget))
    UInputKeyBindingSlot* PrimaryKeySlot;

    UPROPERTY(meta = (BindWidget))
    UInputKeyBindingSlot* SecondaryKeySlot;
};
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FactoryUIBase.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API UFactoryUIBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	bool isPinned = false;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HeaderText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FButtonStyle PinOnButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FButtonStyle PinOffButtonStyle;

	UPROPERTY(meta = (BindWidget))
	class UNamedSlot* FactoryUISlot;

	UPROPERTY(meta = (BindWidget))
	class UButton* PinToggleButton;

	UFUNCTION()
	void PinToggle();

public:
	void SetHeaderText(FString TextToSet);

	bool GetIsPinned() const;
};

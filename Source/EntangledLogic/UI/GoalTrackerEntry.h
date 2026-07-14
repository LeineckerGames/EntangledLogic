#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoalTrackerEntry.generated.h"

UENUM()
enum class EGoalTrackerEntryState : uint8
{
	Preview,
	Collapsed,
	Expanded
};


UCLASS()
class ENTANGLEDLOGIC_API UGoalTrackerEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGoalTrackerEntryState State;

	UPROPERTY()
	bool bIsPinned;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExpandButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PinButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ExpandSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* PinSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *DescriptionText;

	virtual void NativeConstruct() override;

	void TogglePin();

	void ToggleState();

	void UpdateState();
};

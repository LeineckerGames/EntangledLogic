#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EntangledLogic/Core/Framework/ProgressionGoalsDataStructs.h"
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

	UFUNCTION()
	virtual void SynchronizeProperties() override;

	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	void TogglePin();

	UFUNCTION()
	void ToggleState();

	UFUNCTION()
	void UpdateState();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EProgressionGoals Goal;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoalTitle;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StateText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProgressText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* GoalProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DescriptionText;

	UFUNCTION()
	void UpdateDisplay();
};

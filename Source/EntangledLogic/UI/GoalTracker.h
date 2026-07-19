#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EntangledLogic/UI/GoalTrackerEntry.h"
#include "GoalTracker.generated.h"

UENUM()
enum class EGoalTrackerState : uint8
{
	Collapsed,
	Expanded
};

UCLASS()
class ENTANGLEDLOGIC_API UGoalTracker : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGoalTrackerState State;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExpandButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* TitleSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ButtonSwitcher;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGoalTrackerEntry> GoalEntryClass;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* GoalScrollBox;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void ToggleState();

	UFUNCTION()
	void UpdateState();

	UFUNCTION()
	void Expand();

	UFUNCTION()
	void Collapse();

	UFUNCTION()
	void PopulateGoals();

	UFUNCTION()
	void UpdateGoals();
};

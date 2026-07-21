#include "EntangledLogic/UI/GoalTrackerEntry.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ProgressBar.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"

void UGoalTrackerEntry::SynchronizeProperties()
{
	UpdateState();
}

void UGoalTrackerEntry::NativeConstruct()
{
	if (ExpandButton) ExpandButton->OnClicked.AddDynamic(this, &UGoalTrackerEntry::ToggleState);
	if (PinButton) PinButton->OnClicked.AddDynamic(this, &UGoalTrackerEntry::TogglePin);
	UpdateState();
	UpdateDisplay();
}

void UGoalTrackerEntry::UpdateDisplay()
{
	UFactorySubsystem* FactorySubsystem = GetWorld()->GetSubsystem<UFactorySubsystem>();
	if (FactorySubsystem)
	{
		FProgressionGoal* GoalStruct = FactorySubsystem->PersistantStats.CurrentProgressionGoals.FindByKey(Goal);

		if (GoalStruct)
		{
			GoalTitle->SetText(FText::FromString(GoalStruct->ProgressionGoalsData.GoalTitle));
			DescriptionText->SetText(FText::FromString(GoalStruct->ProgressionGoalsData.GoalDescription));

			StateText->SetText(FText::FromString(GoalStruct->ProgressionGoalsData.AcceptedState.ConvertToString()));

			int32 current = GoalStruct->ProgressionGoalCount;
			int32 total = GoalStruct->ProgressionGoalsData.RequiredStatesAmount;

			FString GoalCountString = FString::Printf(TEXT("%d/%d"), current, total);
			ProgressText->SetText(FText::FromString(GoalCountString));

			GoalProgressBar->SetPercent(((float)current) / total);
		}
	}
}

void UGoalTrackerEntry::TogglePin()
{
	if (bIsPinned) PinSwitcher->SetActiveWidgetIndex(0);
	else PinSwitcher->SetActiveWidgetIndex(1);
	bIsPinned = !bIsPinned;
}

void UGoalTrackerEntry::ToggleState()
{
	switch (State) {
	case EGoalTrackerEntryState::Collapsed:
		State = EGoalTrackerEntryState::Expanded;
		UpdateState();
		break;
	case EGoalTrackerEntryState::Expanded:
		State = EGoalTrackerEntryState::Collapsed;
		UpdateState();
		break;
	}
}

void UGoalTrackerEntry::UpdateState()
{
	switch (State) {
	case EGoalTrackerEntryState::Preview:
		ExpandButton->SetVisibility(ESlateVisibility::Collapsed);
		PinButton->SetVisibility(ESlateVisibility::Collapsed);
		DescriptionText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EGoalTrackerEntryState::Collapsed:
		ExpandButton->SetVisibility(ESlateVisibility::Visible);
		PinButton->SetVisibility(ESlateVisibility::Visible);
		ExpandSwitcher->SetActiveWidgetIndex(0);
		DescriptionText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EGoalTrackerEntryState::Expanded:
		ExpandButton->SetVisibility(ESlateVisibility::Visible);
		PinButton->SetVisibility(ESlateVisibility::Visible);
		ExpandSwitcher->SetActiveWidgetIndex(1);
		DescriptionText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		break;
	}
}
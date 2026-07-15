#include "EntangledLogic/UI/GoalTrackerEntry.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UGoalTrackerEntry::NativeConstruct()
{
	if (ExpandButton) ExpandButton->OnClicked.AddDynamic(this, &UGoalTrackerEntry::ToggleState);
	if (PinButton) PinButton->OnClicked.AddDynamic(this, &UGoalTrackerEntry::TogglePin);
	UpdateState();
}

void UGoalTrackerEntry::TogglePin()
{
	if (bIsPinned) PinSwitcher->SetActiveWidgetIndex(0);
	else PinSwitcher->SetActiveWidgetIndex(1);
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
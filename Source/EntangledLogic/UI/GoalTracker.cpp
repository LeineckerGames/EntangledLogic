#include "EntangledLogic/UI/GoalTracker.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"

void UGoalTracker::NativeConstruct()
{
	if (ExpandButton) ExpandButton->OnClicked.AddDynamic(this, &UGoalTracker::ToggleState);
	UpdateState();
}

void UGoalTracker::ToggleState()
{
	UE_LOG(LogTemp, Display, TEXT("Toggling Goal Tracker state"));
	if (State == EGoalTrackerState::Collapsed)
		State = EGoalTrackerState::Expanded;
	else State = EGoalTrackerState::Collapsed;
	UpdateState();
}

void UGoalTracker::UpdateState()
{
	UE_LOG(LogTemp, Display, TEXT("Updataing Goal Tracker state"));
	(State == EGoalTrackerState::Collapsed) ? Collapse() : Expand();
}

void UGoalTracker::Expand()
{
	UE_LOG(LogTemp, Display, TEXT("Expanding Goal Tracker"));
	TitleSwitcher->SetActiveWidgetIndex(1);
	ButtonSwitcher->SetActiveWidgetIndex(1);
	GoalScrollBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGoalTracker::Collapse()
{
	UE_LOG(LogTemp, Display, TEXT("Collapsing Goal Tracker"));
	TitleSwitcher->SetActiveWidgetIndex(0);
	ButtonSwitcher->SetActiveWidgetIndex(0);
	GoalScrollBox->SetVisibility(ESlateVisibility::Collapsed);

}

void UGoalTracker::PopulateGoals()
{
	UFactorySubsystem* FactorySubsystem = GetWorld()->GetSubsystem<UFactorySubsystem>();
	if (FactorySubsystem)
	{
		GoalScrollBox->ClearChildren();

		for (FProgressionGoal goal : FactorySubsystem->PersistantStats.CurrentProgressionGoals)
		{
			UGoalTrackerEntry* entry = CreateWidget<UGoalTrackerEntry>(this, GoalEntryClass);
			entry->Goal = goal.ProgressionGoal;
			entry->UpdateDisplay();
		}
	}
}

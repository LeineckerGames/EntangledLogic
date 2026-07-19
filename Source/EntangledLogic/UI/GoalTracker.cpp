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
	if (State == EGoalTrackerState::Collapsed)
		State = EGoalTrackerState::Expanded;
	else State = EGoalTrackerState::Collapsed;
	UpdateState();
}

void UGoalTracker::UpdateState()
{
	(State == EGoalTrackerState::Collapsed) ? Collapse() : Expand();
}

void UGoalTracker::Expand()
{
	UpdateGoals();
	TitleSwitcher->SetActiveWidgetIndex(1);
	ButtonSwitcher->SetActiveWidgetIndex(1);
	GoalScrollBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGoalTracker::Collapse()
{
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

		for (FProgressionGoal Goal : FactorySubsystem->PersistantStats.CurrentProgressionGoals)
		{
			UGoalTrackerEntry* Entry = CreateWidget<UGoalTrackerEntry>(this, GoalEntryClass);
			Entry->Goal = Goal.ProgressionGoal;
			Entry->UpdateDisplay();
			GoalScrollBox->AddChild(Entry);
		}
	}
}

void UGoalTracker::UpdateGoals()
{
	for (auto Child : GoalScrollBox->GetAllChildren())
	{
		UGoalTrackerEntry* Entry = Cast<UGoalTrackerEntry>(Child);
		if (Entry) Entry->UpdateDisplay();
	}
}

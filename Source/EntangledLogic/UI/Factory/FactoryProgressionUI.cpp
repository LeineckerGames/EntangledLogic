#include "FactoryProgressionUI.h"
#include "Components/TextBlock.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"

void UFactoryProgressionUI::UpdateProgressionUI()
{
	UFactorySubsystem* FactorySubsytem = GetWorld()->GetSubsystem<UFactorySubsystem>();
	if (FactorySubsytem)
	{
		if (FactorySubsytem->PersistantStats.CurrentProgressionGoals.Num() > 1)
		{
			// Set ProgressionCountText
			FString GoalCountString = FString::Printf(TEXT("%d / %d"), FactorySubsytem->PersistantStats.CurrentProgressionGoals[0].ProgressionGoalCount
				, FactorySubsytem->PersistantStats.CurrentProgressionGoals[0].ProgressionGoalsData.RequiredStatesAmount);
			ProgressionCountText->SetText(FText::FromString(GoalCountString));

			// Set State Info
			QubitStateGoalText->SetText(FText::FromString(FactorySubsytem->PersistantStats.CurrentProgressionGoals[0].ProgressionGoalsData.AcceptedStateString));
		}
	}
	
}
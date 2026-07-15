#include "FactoryProgressionUI.h"
#include "Components/TextBlock.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"

void UFactoryProgressionUI::UpdateProgressionUI()
{
	UFactorySubsystem* FactorySubsytem = GetWorld()->GetSubsystem<UFactorySubsystem>();
	if (FactorySubsytem)
	{
		// Set ProgressionCountText
		FString GoalCountString = FString::Printf(TEXT("%d / %d"), FactorySubsytem->PersistantStats.CurrentGoalAcceptedStatesCount
																, FactorySubsytem->CurrentGoalRequiredStatesCount);
		ProgressionCountText->SetText(FText::FromString(GoalCountString));

		// Set State Info
		QubitStateGoalText->SetText(FText::FromString(FactorySubsytem->CurrentRequiredStateString));
	}
	
}
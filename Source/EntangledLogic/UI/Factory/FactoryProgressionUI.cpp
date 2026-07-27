#include "FactoryProgressionUI.h"
#include "Components/TextBlock.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"

void UFactoryProgressionUI::NativeConstruct()
{
	Super::NativeConstruct();
	UFactorySubsystem* FactorySubsytem = GetWorld()->GetSubsystem<UFactorySubsystem>();
	if (FactorySubsytem)
	{
		FactorySubsytem->UpdateProgressionUIs.AddUObject(this, &UFactoryProgressionUI::UpdateProgressionUI);
	}

}

void UFactoryProgressionUI::UpdateProgressionUI(int32 ProgressionGoalIndex)
{
	UFactorySubsystem* FactorySubsytem = GetWorld()->GetSubsystem<UFactorySubsystem>();
	if (FactorySubsytem)
	{
		if (FactorySubsytem->PersistantStats.CurrentProgressionGoals.Num() > 0)
		{
			FProgressionGoal CurrentGoal = FactorySubsytem->PersistantStats.CurrentProgressionGoals[ProgressionGoalIndex];
			// Set ProgressionCountText
			FString GoalCountString = FString::Printf(TEXT("%d / %d"), CurrentGoal.ProgressionGoalCount, CurrentGoal.ProgressionGoalsData.RequiredStatesAmount);
			ProgressionCountText->SetText(FText::FromString(GoalCountString));

			// Set State Info
			QubitStateGoalText->SetText(FText::FromString(CurrentGoal.ProgressionGoalsData.AcceptedState.ConvertToString(true)));
		
			// Set Current Goal Text
			CurrentGoalText->SetText(FText::FromString(CurrentGoal.ProgressionGoalsData.GoalTitle));
		}
	}
	
}
#include "FactorySubsystem.h"
#include "EntangledLogic/Core/Framework/UnlockablesEnum.h"
#include "EntangledLogic/Core/Framework/FactorySaveGame.h"
#include "EntangledLogic/Core/DevSettings/FactorySettings.h"
#include "EntangledLogic/Core/Framework/ProgressionGoalsDataAsset.h"
#include "EntangledLogic/Core/Subsystems/SavingLoadingSubsystem.h"
#include "EntangledLogic/Core/Subsystems/GlobalAudioSubsystem.h"
#include "EntangledLogic/UI/PlayerHUD.h"
#include "TimerManager.h"

void UFactorySubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);

	// Register Subsystem for saving and loading
	UWorld* World = GetWorld();
	if (World)
	{
		USavingLoadingSubsystem* SavingLoading = World->GetGameInstance()->GetSubsystem<USavingLoadingSubsystem>();
		if (SavingLoading)
		{
			UE_LOG(LogTemp, Display, TEXT("Registering FSS to Saving and Loading"));
			SavingLoading->RegisterUObjectToSavingLoading(this);
		}
	}

	if (World)
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, this, &UFactorySubsystem::SetTickTrue, 2, true);
	}

}

void UFactorySubsystem::SetProgressionGoalCount(FProgressionGoal &ProgressionGoal, int32 ValueToSet)
{
	int32 requirement = ProgressionGoal.ProgressionGoalsData.RequiredStatesAmount;
	ProgressionGoal.ProgressionGoalCount = ValueToSet >= requirement? requirement : ValueToSet;
	UE_LOG(LogTemp, Display, TEXT("Setting Progression Goal %s to value %d"),
								  *UEnum::GetValueAsString(ProgressionGoal.ProgressionGoal),
								  ProgressionGoal.ProgressionGoalCount)

	if (ProgressionGoal.ProgressionGoalCount >= requirement)
	{
		FProgressionGoal GoalToRemove = ProgressionGoal;
		// Add all the unlocks
		for (EUnlockables CurrentUnlock : ProgressionGoal.ProgressionGoalsData.UnlockablesOnCompletion)
		{
			UnlockProgression(CurrentUnlock);
		}

		// Add next progressions
		for (auto NextGoal : ProgressionGoal.ProgressionGoalsData.NextProgressionGoals) {
			FProgressionGoalsData* NextProgressionGoal = ProgressionGoalsDataAsset->ProgressionGoals.Find(NextGoal);
			if (NextProgressionGoal)
			{
				UE_LOG(LogTemp, Display, TEXT("Unlocking Next Progression Goal"))
				AddProgressionGoal(NextGoal);
			}
		}

		// Remove old goal
		PersistantStats.CurrentProgressionGoals.Remove(GoalToRemove);

		RepopulateWidgets();
	}
	else
	{
		UpdateWidgets();
	}
}

void UFactorySubsystem::AddProgressionGoal(EProgressionGoals ProgressionGoalToAdd)
{
	FProgressionGoalsData* ProgressionGoalData = ProgressionGoalsDataAsset->ProgressionGoals.Find(ProgressionGoalToAdd);
	if (ProgressionGoalData)
	{
		FProgressionGoal NewProgressionGoal;
		NewProgressionGoal.ProgressionGoalsData = *ProgressionGoalData;
		NewProgressionGoal.ProgressionGoal = ProgressionGoalToAdd;
		PersistantStats.CurrentProgressionGoals.Add(NewProgressionGoal);
		UE_LOG(LogTemp, Display, TEXT("Added Progression Num() = %d"), PersistantStats.CurrentProgressionGoals.Num())
	}
}

bool UFactorySubsystem::GetTickPaused() const
{
	return isTickPaused;
}

TMap<EUnlockables, bool> UFactorySubsystem::GetUnlockablesMap() const
{
	return UnlockablesMap;
}

bool UFactorySubsystem::CheckIfUnlocked(EUnlockables UnlockToCheck)
{
	if (UnlockToCheck == EUnlockables::Unlocked)
	{
		return true;
	}

	bool* isUnlockedPointer = UnlockablesMap.Find(UnlockToCheck);
	if (isUnlockedPointer)
	{
		bool isUnlocked = *isUnlockedPointer;
		return isUnlocked;
	}
	return false;
}

void UFactorySubsystem::UnlockProgression(EUnlockables ProgressionToUnlock)
{
	UnlockablesMap.Emplace(ProgressionToUnlock, true);
	// RepopulateWidgets();
}

// Refresh widgets, clearing and repopulating dynamic containers
void UFactorySubsystem::RepopulateWidgets()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			if (PlayerHUD)
			{
				PlayerHUD->RepopulateFactorySelectionWidget();
				PlayerHUD->RepopulateGoalTrackerWidget();
			}
		}
	}
}

// Refresh widget data without repopulating dynamic containers
void UFactorySubsystem::UpdateWidgets()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			if (PlayerHUD)
			{
				PlayerHUD->UpdateGoalTrackerWidget();
			}
		}
	}
}

void UFactorySubsystem::SetTickPaused(bool TickPausedValue)
{
	isTickPaused = TickPausedValue;

	// Sets the BG music to be filtered or not
	if (UWorld* World = GetWorld())
	{
		UGlobalAudioSubsystem* GlobalAudio = World->GetGameInstance()->GetSubsystem<UGlobalAudioSubsystem>();
		if (GlobalAudio)
		{
			if (isTickPaused)
			{
				GlobalAudio->SetPauseWet();
			}
			else
			{
				GlobalAudio->SetPauseDry();
			}
		}
	}
}

void UFactorySubsystem::SetTickTrue()
{
	if (isTickPaused)
	{
		CanTick = false;
	}
	else
	{
		CanTick = true;
	}
}

void UFactorySubsystem::SetCanTick(bool CanTickValue)
{
	CanTick = CanTickValue;
}

void UFactorySubsystem::Tick(float DeltaTime)
{
	if (CanTick)
	{
		CanTick = false;
		OnFactoryTick.Broadcast();
	}
}

TStatId UFactorySubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFactorySubsystem, STATGROUP_Tickables);
}

void UFactorySubsystem::SaveData(UFactorySaveGame* SaveGame)
{
	UE_LOG(LogTemp, Display, TEXT("Saving PersistantStats Data"));
	SaveGame->PersistantStats = PersistantStats;
	SaveGame->UnlockablesMap = UnlockablesMap;
}

void UFactorySubsystem::LoadData(UFactorySaveGame* SaveGame)
{
	UE_LOG(LogTemp, Display, TEXT("Loading PersistantStats Data"));
	PersistantStats = SaveGame->PersistantStats;
	UnlockablesMap = SaveGame->UnlockablesMap;
	RepopulateWidgets();
}
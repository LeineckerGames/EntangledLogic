#include "FactorySubsystem.h"
#include "EntangledLogic/Core/Framework/UnlockablesEnum.h"
#include "EntangledLogic/Core/Framework/FactorySaveGame.h"
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

	// Starts music
	UGlobalAudioSubsystem* GlobalAudio = World->GetGameInstance()->GetSubsystem<UGlobalAudioSubsystem>();
	if (GlobalAudio)
	{
		GlobalAudio->StartBackgroundMusic();
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
	RepopulateFactorySelectionWidget();
}

void UFactorySubsystem::RepopulateFactorySelectionWidget()
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
			}
		}
	}
}

void UFactorySubsystem::SetTickPaused(bool TickPausedValue)
{
	isTickPaused = TickPausedValue;
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
		UE_LOG(LogTemp, Display, TEXT("FACTORY TICK"));
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
}

void UFactorySubsystem::LoadData(UFactorySaveGame* SaveGame)
{
	UE_LOG(LogTemp, Display, TEXT("Loading PersistantStats Data"));
	PersistantStats = SaveGame->PersistantStats;

}
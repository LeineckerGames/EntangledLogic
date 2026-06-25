#include "GlobalAudioSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"
#include "EntangledLogic/Core/DevSettings/GlobalAudioSettings.h"
#include "Components/AudioComponent.h"


void UGlobalAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGlobalAudioSubsystem::StartBackgroundAmbience()
{
	UWorld* World = GetWorld();

	const UGlobalAudioSettings* AudioSettings = GetDefault<UGlobalAudioSettings>();
	if (AudioSettings && AudioSettings->BackgroundMusic && AudioSettings->MuteMusic != true)
	{
		USoundBase* BackgroundMusic = AudioSettings->BackgroundMusic.LoadSynchronous();

		if (BackgroundMusic && World)
		{
			UE_LOG(LogTemp, Display, TEXT("Playing Music"));
			BackgroundMusicAudioComponent = UGameplayStatics::SpawnSound2D(World, BackgroundMusic);

			UMetaSoundOutputSubsystem* OutputSubsystem = GetWorld()->GetSubsystem<UMetaSoundOutputSubsystem>();
			if (OutputSubsystem)
			{
				FOnMetasoundOutputValueChangedNative OnOutputChangedDelegate;
				OnOutputChangedDelegate.BindUObject(this, &UGlobalAudioSubsystem::OnMetaSoundOutputChanged);
				OutputSubsystem->WatchOutput(BackgroundMusicAudioComponent, FName("MusicFinished"), OnOutputChangedDelegate);
				StartMusicTimer(45);
			}
		}
	}



}

void UGlobalAudioSubsystem::TriggerMusic()
{
	BackgroundMusicAudioComponent->SetTriggerParameter(FName("PlayMusic"));
}

void UGlobalAudioSubsystem::StartMusicTimer()
{
	const UGlobalAudioSettings* AudioSettings = GetDefault<UGlobalAudioSettings>();
	if (AudioSettings)
	{
		int32 TimeBetweenNextSong = FMath::RandRange(AudioSettings->MinTimeBetweenSongs, AudioSettings->MaxTimeBetweenSongs);
		UE_LOG(LogTemp, Display, TEXT("TimeBetweenNextSong = %d"), TimeBetweenNextSong);

		UWorld* World = GetWorld();
		if (World)
		{
			FTimerHandle TimerHandle;
			World->GetTimerManager().SetTimer(TimerHandle, this, &UGlobalAudioSubsystem::TriggerMusic, TimeBetweenNextSong, false);
		}
	}
}

void UGlobalAudioSubsystem::StartMusicTimer(int32 TimeOverride)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, this, &UGlobalAudioSubsystem::TriggerMusic, TimeOverride, false);
	}
}

void UGlobalAudioSubsystem::OnMetaSoundOutputChanged(FName OutputName, const FMetaSoundOutput& Output)
{
	//Start Timer
	UE_LOG(LogTemp, Display, TEXT("Starting Music Timer"));
	StartMusicTimer();
}


void UGlobalAudioSubsystem::SetPauseWet()
{
	BackgroundMusicAudioComponent->SetFloatParameter(FName("PausedDryWet"), 1.0f);
}

void UGlobalAudioSubsystem::SetPauseDry()
{
	BackgroundMusicAudioComponent->SetFloatParameter(FName("PausedDryWet"), 0.0f);
}
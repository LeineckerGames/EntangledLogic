#include "SettingsTabAudio.h"
#include "Components/Slider.h"
#include "CoreMinimal.h"
#include "Misc/ConfigCacheIni.h"

void USettingsTabAudio::NativeConstruct()
{
    Super::NativeConstruct();

	// Bind the slider value changed events to their respective functions
    if (MasterVolumeSlider) MasterVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnMasterVolumeChanged);
    if (MusicVolumeSlider) MusicVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnMusicVolumeChanged);
    if (AmbienceVolumeSlider) AmbienceVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnAmbienceVolumeChanged);
    if (SFXVolumeSlider) SFXVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnSFXVolumeChanged);

	// Initialize the sliders with the current settings values
    UAudioModulationStatics::ActivateBusMix(GetWorld(), ControlBusMix);
    LoadVolume();
}

// Master Volume
void USettingsTabAudio::OnMasterVolumeChanged(float Value)
{
    SetModulationVolume(MasterBus, Value);
    SaveVolume(Value, MusicVolume, AmbienceVolume, SFXVolume);
}

// Music Volume
void USettingsTabAudio::OnMusicVolumeChanged(float Value)
{
    SetModulationVolume(MusicBus, Value);
    SaveVolume(MasterVolume, Value, AmbienceVolume, SFXVolume);
}

// Ambience Volume
void USettingsTabAudio::OnAmbienceVolumeChanged(float Value)
{
    SetModulationVolume(AmbienceBus, Value);
    SaveVolume(MasterVolume, MusicVolume, Value, SFXVolume);
}

// SFX Volume
void USettingsTabAudio::OnSFXVolumeChanged(float Value)
{
    SetModulationVolume(SFXBus, Value);
    SaveVolume(MasterVolume, MusicVolume, AmbienceVolume, Value);
}

// Volume Changer function
void USettingsTabAudio::SetModulationVolume(USoundControlBus* TargetBus, float Volume)
{
    if (!ControlBusMix || !TargetBus)
    {
        UE_LOG(LogTemp, Warning, TEXT("Bus Mix or Control Bus asset is missing."));
        return;
    }

    // Clamp volume between 0.0 (silent) and 1.0 (full volume)
    float ClampedVolume = FMath::Clamp(Volume, 0.0f, 1.0f);

    // Create the "Stage" which holds the target value for our specific bus
    FSoundControlBusMixStage MixStage;
    MixStage.Bus = TargetBus;
    MixStage.Value.TargetValue = ClampedVolume;

    // How long it takes to fade to the new volume (in seconds)
    MixStage.Value.AttackTime = 0.1f;
    MixStage.Value.ReleaseTime = 0.1f;

    // Package it into an array (Mixes can update multiple buses at once)
    TArray<FSoundControlBusMixStage> Stages;
    Stages.Add(MixStage);

    // Update the Mix
    UAudioModulationStatics::UpdateMix(GetWorld(), ControlBusMix, Stages);
}

// Save Volume Helper
void USettingsTabAudio::SaveVolume(float Master, float Music, float Ambience, float SFX)
{
    if (GConfig)
    {
        GConfig->SetFloat(TEXT("AudioSettings"), TEXT("Master"), Master, GGameUserSettingsIni);
        GConfig->SetFloat(TEXT("AudioSettings"), TEXT("Music"), Music, GGameUserSettingsIni);
        GConfig->SetFloat(TEXT("AudioSettings"), TEXT("Ambience"), Ambience, GGameUserSettingsIni);
        GConfig->SetFloat(TEXT("AudioSettings"), TEXT("SFX"), SFX, GGameUserSettingsIni);
        GConfig->Flush(false, GGameUserSettingsIni);
    }
}

// Load Volume Helper
void USettingsTabAudio::LoadVolume()
{
    // Default values
    MasterVolume = 1.0f;
    MusicVolume = 1.0f;
    AmbienceVolume = 1.0f;
    SFXVolume = 1.0f;

    if (GConfig)
    {
        GConfig->GetFloat(TEXT("AudioSettings"), TEXT("Master"), MasterVolume, GGameUserSettingsIni);
        GConfig->GetFloat(TEXT("AudioSettings"), TEXT("Music"), MusicVolume, GGameUserSettingsIni);
        GConfig->GetFloat(TEXT("AudioSettings"), TEXT("Ambience"), AmbienceVolume, GGameUserSettingsIni);
        GConfig->GetFloat(TEXT("AudioSettings"), TEXT("SFX"), SFXVolume, GGameUserSettingsIni);
    }

    if (MasterVolumeSlider) MasterVolumeSlider->SetValue(MasterVolume);
    if (MusicVolumeSlider) MusicVolumeSlider->SetValue(MusicVolume);
    if (AmbienceVolumeSlider) AmbienceVolumeSlider->SetValue(AmbienceVolume);
    if (SFXVolumeSlider) SFXVolumeSlider->SetValue(SFXVolume);
}
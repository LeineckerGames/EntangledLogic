#include "SettingsTabAudio.h"
#include "Components/Slider.h"

void USettingsTabAudio::NativeConstruct()
{
    Super::NativeConstruct();

	// Bind the slider value changed events to their respective functions
    if (MasterVolumeSlider) MasterVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnMasterVolumeChanged);
    if (MusicVolumeSlider) MusicVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnMusicVolumeChanged);
    if (DialogVolumeSlider) DialogVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnDialogVolumeChanged);
    if (SFXVolumeSlider) SFXVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnSFXVolumeChanged);

    // Init the sliders with the current bus values from the save or init them at a 0.7
    InitAudio();
}

// Master Volume
void USettingsTabAudio::OnMasterVolumeChanged(float Value)
{
    SetBusVolume(MasterBusAsset, Value);
}

// Music Volume
void USettingsTabAudio::OnMusicVolumeChanged(float Value)
{
    SetBusVolume(MusicBusAsset, Value);
}

// Dialog Volume
void USettingsTabAudio::OnDialogVolumeChanged(float Value)
{
    SetBusVolume(DialogBusAsset, Value);
}

// SFX Volume
void USettingsTabAudio::OnSFXVolumeChanged(float Value)
{
    SetBusVolume(SFXBusAsset, Value);
}

// Initialize audio settings
void USettingsTabAudio::InitAudio()
{
	// If Sound settings in save exist, load them and set the sliders accordingly. Otherwise, set default values.
    /*
    if ()
    {
        if (MasterVolumeSlider) MasterVolumeSlider->SetValue();
        if (MusicVolumeSlider) MusicVolumeSlider->SetValue();
        if (DialogVolumeSlider) DialogVolumeSlider->SetValue();
        if (SFXVolumeSlider) SFXVolumeSlider->SetValue();
    }
    else {
        if (MasterVolumeSlider) MasterVolumeSlider->SetValue(0.7f);
        if (MusicVolumeSlider) MusicVolumeSlider->SetValue(0.7f);
        if (DialogVolumeSlider) DialogVolumeSlider->SetValue(0.7f);
        if (SFXVolumeSlider) SFXVolumeSlider->SetValue(0.7f);
    }
    */
}

// Helper function to set the volume of a specific sound control bus
void USettingsTabAudio::SetBusVolume(USoundControlBus* Bus, float Volume)
{
    if (!Bus || !SettingsBusMixAsset || !GetWorld()) return;

    // Create a temporary modulation stage configuration
    FSoundControlBusMixStage MixStage;
    MixStage.Bus = Bus;
    MixStage.Value.TargetValue = FMath::Clamp(Volume, 0.0f, 1.0f);

    TArray<FSoundControlBusMixStage> Stages;
    Stages.Add(MixStage);

    // Update the bus value globally across the engine instance instantly
    UAudioModulationStatics::UpdateMix(GetWorld(), SettingsBusMixAsset, Stages);
}
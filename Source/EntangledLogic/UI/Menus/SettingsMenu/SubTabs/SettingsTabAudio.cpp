#include "SettingsTabAudio.h"
#include "Components/Slider.h"
#include "EntangledLogic/Core/Framework/CustomGameUserSettings.h"

void USettingsTabAudio::NativeConstruct()
{
    Super::NativeConstruct();

	// Bind the slider value changed events to their respective functions
    if (MasterVolumeSlider) MasterVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnMasterVolumeChanged);
    if (MusicVolumeSlider) MusicVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnMusicVolumeChanged);
    if (DialogVolumeSlider) DialogVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnDialogVolumeChanged);
    if (SFXVolumeSlider) SFXVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsTabAudio::OnSFXVolumeChanged);

	// Initialize the sliders with the current settings values
    if (UCustomGameUserSettings* Settings = UCustomGameUserSettings::GetCustomGameUserSettings())
    {
        if (MasterVolumeSlider) MasterVolumeSlider->SetValue(Settings->MasterVolume);
        if (MusicVolumeSlider) MusicVolumeSlider->SetValue(Settings->MusicVolume);
        if (DialogVolumeSlider) DialogVolumeSlider->SetValue(Settings->DialogVolume);
        if (SFXVolumeSlider) SFXVolumeSlider->SetValue(Settings->SFXVolume);
    }
}

// Master Volume
void USettingsTabAudio::OnMasterVolumeChanged(float Value)
{
    if (UCustomGameUserSettings* Settings = UCustomGameUserSettings::GetCustomGameUserSettings())
    {
        Settings->MasterVolume = Value;
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }
}

// Music Volume
void USettingsTabAudio::OnMusicVolumeChanged(float Value)
{
    if (UCustomGameUserSettings* Settings = UCustomGameUserSettings::GetCustomGameUserSettings())
    {
        Settings->MusicVolume = Value;
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }
}

// Dialog Volume
void USettingsTabAudio::OnDialogVolumeChanged(float Value)
{
    if (UCustomGameUserSettings* Settings = UCustomGameUserSettings::GetCustomGameUserSettings())
    {
        Settings->DialogVolume = Value;
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }
}

// SFX Volume
void USettingsTabAudio::OnSFXVolumeChanged(float Value)
{
    if (UCustomGameUserSettings* Settings = UCustomGameUserSettings::GetCustomGameUserSettings())
    {
        Settings->SFXVolume = Value;
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }
}
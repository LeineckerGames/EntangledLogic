#include "CustomGameUserSettings.h"

//
UCustomGameUserSettings::UCustomGameUserSettings()
{
    SetToDefaults();
}

// Returns the GameEngine reference
UCustomGameUserSettings* UCustomGameUserSettings::GetCustomGameUserSettings()
{
    return Cast<UCustomGameUserSettings>(GEngine ? GEngine->GetGameUserSettings() : nullptr);
}

// List of default values incase user has not set any or it is first boot
void UCustomGameUserSettings::SetToDefaults()
{
    Super::SetToDefaults();

    // Default fallback values
    MasterVolume = 0.7f;
    MusicVolume = 0.7f;
    DialogVolume = 0.7f;
    SFXVolume = 0.7f;

    // Controls

    // Gameplay
    // Mostly a placeholder for now, but could be used to turn on and off tutorials, or change gameplay settings.

    // Graphics

    // Language
    // Mostly a placeholder for now, but could be used to turn on and off subtitles, or change the language of the game if localization is implemented.
}

// Overrided ApplySettings to broadcast changed audio settings
void UCustomGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
    Super::ApplySettings(bCheckForCommandLineOverrides);

    // Broadcast updates so the Audio System can apply the Modulation parameters
    OnMasterVolumeChanged.Broadcast(MasterVolume);
    OnMusicVolumeChanged.Broadcast(MusicVolume);
    OnDialogVolumeChanged.Broadcast(DialogVolume);
    OnSFXVolumeChanged.Broadcast(SFXVolume);
}
/*
// Change Master Volume
void UCustomGameUserSettings::SetMasterVolume(float InVolume)
{
    float ClampedVolume = FMath::Clamp(InVolume, 0.0f, 1.0f);
    if (MasterVolume != ClampedVolume)
    {
        MasterVolume = ClampedVolume;
        OnMasterVolumeChanged.Broadcast(MasterVolume);
    }
}

// Change Music Volume
void UCustomGameUserSettings::SetMusicVolume(float InVolume)
{
    float ClampedVolume = FMath::Clamp(InVolume, 0.0f, 1.0f);
    if (MusicVolume != ClampedVolume)
    {
        MusicVolume = ClampedVolume;
        OnMusicVolumeChanged.Broadcast(MusicVolume);
    }
}

// Change Dialog Volume
void UCustomGameUserSettings::SetDialogVolume(float InVolume)
{
    float ClampedVolume = FMath::Clamp(InVolume, 0.0f, 1.0f);
    if (DialogVolume != ClampedVolume)
    {
        DialogVolume = ClampedVolume;
        OnDialogVolumeChanged.Broadcast(DialogVolume);
    }
}

// Change SFX Volume
void UCustomGameUserSettings::SetSFXVolume(float InVolume)
{
    float ClampedVolume = FMath::Clamp(InVolume, 0.0f, 1.0f);
    if (SFXVolume != ClampedVolume)
    {
        SFXVolume = ClampedVolume;
        OnSFXVolumeChanged.Broadcast(SFXVolume);
    }
}
*/
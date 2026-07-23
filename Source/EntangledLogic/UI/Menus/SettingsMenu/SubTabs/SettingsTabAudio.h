#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioModulationStatics.h"
#include "SettingsTabAudio.generated.h"

class USoundControlBus;
class USoundControlBusMix;
class USlider;

UCLASS()
class ENTANGLEDLOGIC_API USettingsTabAudio : public UUserWidget
{
    GENERATED_BODY()

private:
    float MasterVolume;
    float MusicVolume;
    float AmbienceVolume;
    float SFXVolume;

protected:
    virtual void NativeConstruct() override;

    // Audio Settings
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio|Modulation")
    USoundControlBusMix* ControlBusMix;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio|Modulation")
    USoundControlBus* MasterBus;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio|Modulation")
    USoundControlBus* MusicBus;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio|Modulation")
    USoundControlBus* AmbienceBus;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio|Modulation")
    USoundControlBus* SFXBus;

    UPROPERTY(meta = (BindWidget))
    USlider* MasterVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* MusicVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* AmbienceVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* SFXVolumeSlider;

    // On change functions
    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    UFUNCTION()
    void OnMusicVolumeChanged(float Value);

    UFUNCTION()
    void OnAmbienceVolumeChanged(float Value);

    UFUNCTION()
    void OnSFXVolumeChanged(float Value);

    UFUNCTION()
    void SetModulationVolume(USoundControlBus* TargetBus, float Volume);

    UFUNCTION()
    void SaveVolume(float Master, float Music, float Ambience, float SFX);

    UFUNCTION()
    void LoadVolume();
};
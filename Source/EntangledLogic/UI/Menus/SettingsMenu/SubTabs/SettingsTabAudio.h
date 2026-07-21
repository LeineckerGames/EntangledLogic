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

protected:
    virtual void NativeConstruct() override;

    // Audio Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float MasterVolume = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float MusicVolume = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float AmbienceVolume = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float SFXVolume = 0.7f;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundControlBusMix* ControlBusMix;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundControlBus* MasterBus;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundControlBus* MusicBus;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundControlBus* AmbienceBus;

    UPROPERTY(EditAnywhere, Category = "Audio")
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
    void SetModulationVolume(USoundControlBus* TargetBus, float Value);

    UFUNCTION()
    void SaveVolume(float Master, float Music, float Ambience, float SFX);

    UFUNCTION()
    void LoadVolume();
};
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioModulationStatics.h"
#include "SettingsTabAudio.generated.h"

//class USoundControlBusMix;
class USlider;

UCLASS()
class ENTANGLEDLOGIC_API USettingsTabAudio : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // Audio Settings
    UPROPERTY(meta = (BindWidget))
    USlider* MasterVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* MusicVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* DialogVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* SFXVolumeSlider;
    
    UPROPERTY(EditDefaultsOnly, Category = "Audio Settings")
    USoundControlBusMix* SettingsBusMixAsset;

    UPROPERTY(EditDefaultsOnly, Category = "Audio Settings")
    USoundControlBus* MasterBusAsset;

    UPROPERTY(EditDefaultsOnly, Category = "Audio Settings")
    USoundControlBus* MusicBusAsset;

    UPROPERTY(EditDefaultsOnly, Category = "Audio Settings")
    USoundControlBus* SFXBusAsset;

    UPROPERTY(EditDefaultsOnly, Category = "Audio Settings")
    USoundControlBus* DialogBusAsset;
    

    // On change functions
    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    UFUNCTION()
    void OnMusicVolumeChanged(float Value);

    UFUNCTION()
    void OnDialogVolumeChanged(float Value);

    UFUNCTION()
    void OnSFXVolumeChanged(float Value);

private:
    void InitAudio();
    void SetBusVolume(USoundControlBus* Bus, float Volume);
};
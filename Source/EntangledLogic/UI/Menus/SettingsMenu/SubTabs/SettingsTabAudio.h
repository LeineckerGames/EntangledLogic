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

    // On change functions
    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    UFUNCTION()
    void OnMusicVolumeChanged(float Value);

    UFUNCTION()
    void OnDialogVolumeChanged(float Value);

    UFUNCTION()
    void OnSFXVolumeChanged(float Value);
};
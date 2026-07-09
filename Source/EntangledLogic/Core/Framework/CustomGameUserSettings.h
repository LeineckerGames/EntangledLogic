#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CustomGameUserSettings.generated.h"

// Delegate allowing external audio managers to respond to changes without tight coupling
DECLARE_MULTICAST_DELEGATE_OneParam(FAudioVolumeChangedDelegate, float Volume);

UCLASS()
class ENTANGLEDLOGIC_API UCustomGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    UCustomGameUserSettings();

    UFUNCTION(BlueprintPure, Category = "Settings")
    static UCustomGameUserSettings* GetCustomGameUserSettings();

	// Audio settings
    UPROPERTY(Config, BlueprintReadWrite, Category = "Settings|Audio")
    float DialogVolume = 0.7f;

    UPROPERTY(Config, BlueprintReadWrite, Category = "Settings|Audio")
    float MasterVolume = 0.7f;

    UPROPERTY(Config, BlueprintReadWrite, Category = "Settings|Audio")
    float MusicVolume = 0.7f;

    UPROPERTY(Config, BlueprintReadWrite, Category = "Settings|Audio")
    float SFXVolume = 0.7f;

    /*
    UFUNCTION(BlueprintCallable, Category = "Settings|Audio")
	void SetMasterVolume(float InVolume);

	UFUNCTION(BlueprintPure, Category = "Settings|Audio")
	float GetMasterVolume() const { return MasterVolume; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Audio")
	void SetMusicVolume(float InVolume);

	UFUNCTION(BlueprintPure, Category = "Settings|Audio")
	float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Audio")
	void SetDialogVolume(float InVolume);

	UFUNCTION(BlueprintPure, Category = "Settings|Audio")
	float GetDialogVolume() const { return DialogVolume; }

	UFUNCTION(BlueprintCallable, Category = "Settings|Audio")
	void SetSFXVolume(float InVolume);

	UFUNCTION(BlueprintPure, Category = "Settings|Audio")
	float GetSFXVolume() const { return SFXVolume; }
    */
    virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
    virtual void SetToDefaults() override;

    FAudioVolumeChangedDelegate OnMasterVolumeChanged;
    FAudioVolumeChangedDelegate OnMusicVolumeChanged;
    FAudioVolumeChangedDelegate OnDialogVolumeChanged;
    FAudioVolumeChangedDelegate OnSFXVolumeChanged;

protected:
    /*
    UPROPERTY(Config)
    float DialogVolume;

    UPROPERTY(Config)
    float MasterVolume;

    UPROPERTY(Config)
    float MusicVolume;

    UPROPERTY(Config)
    float SFXVolume;
    */
};
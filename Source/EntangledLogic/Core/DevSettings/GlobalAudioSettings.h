#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GlobalAudioSettings.generated.h"

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Global Audio Subsystem Settings"))
class ENTANGLEDLOGIC_API UGlobalAudioSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category = "Background Music")
	TSoftObjectPtr<class USoundBase> BackgroundMusic;

	UPROPERTY(Config, EditAnywhere, Category = "Timing")
	int32 MinTimeBetweenSongs = 240;

	UPROPERTY(Config, EditAnywhere, Category = "Timing")
	int32 MaxTimeBetweenSongs = 420;
};

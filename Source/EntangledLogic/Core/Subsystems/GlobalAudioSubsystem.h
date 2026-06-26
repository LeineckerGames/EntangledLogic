#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MetasoundOutputSubsystem.h"
#include "GlobalAudioSubsystem.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API UGlobalAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Transient)
	UAudioComponent* BackgroundMusicAudioComponent;

	FTimerManager TimerManager;

	void OnMetaSoundOutputChanged(FName OutputName, const FMetaSoundOutput& Output);

	UPROPERTY(EditAnywhere, Category = "Background Music")
	class USoundBase* BackgroundMusic;

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void StartBackgroundAmbience();

	void SetBackgroundMusic(class USoundBase* BackgroundMusicToSet);

	void TriggerMusic();

	void StartMusicTimer();

	void StartMusicTimer(int32 TimeOverride);

	void SetPauseWet();

	void SetPauseDry();

};

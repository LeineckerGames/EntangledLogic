#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MetasoundOutputSubsystem.h"
#include "GlobalAudioSubsystem.generated.h"

UCLASS(Abstract, Blueprintable)
class ENTANGLEDLOGIC_API UGlobalAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	class USoundBase* BackgroundMusic;

	UPROPERTY(Transient)
	UAudioComponent* BackgroundMusicAudioComponent;

	FTimerManager TimerManager;

	void OnMetaSoundOutputChanged(FName OutputName, const FMetaSoundOutput& Output);

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void StartBackgroundAmbience();

	void TriggerMusic();

	void StartMusicTimer();

	void StartMusicTimer(int32 TimeOverride);

	void SetPauseWet();

	void SetPauseDry();

};

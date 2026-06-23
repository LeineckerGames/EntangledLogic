#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlobalAudioSubsystem.generated.h"

UCLASS(Abstract, Blueprintable)
class ENTANGLEDLOGIC_API UGlobalAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	class USoundBase* BackgroundMusic;

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void StartBackgroundMusic();

};

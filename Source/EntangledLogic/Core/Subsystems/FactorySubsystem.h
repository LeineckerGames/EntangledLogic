#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FactorySubsystem.generated.h"

enum class EUnlockables : uint8;
UCLASS()
class ENTANGLEDLOGIC_API UFactorySubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
protected:

	bool CanTick = false;
	
	bool isTickPaused = false;

	FTimerManager TimerManager;

	TMap<EUnlockables, bool> UnlockablesMap;

public:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	void SetTickTrue();

	void SetCanTick(bool CanTickValue);

	bool GetTickPaused() const;

	TMap<EUnlockables, bool> GetUnlockablesMap() const;

	bool CheckIfUnlocked(EUnlockables UnlockToCheck);

	void UnlockProgression(EUnlockables ProgressionToUnlock);

	void RepopulateFactorySelectionWidget();

	void SetTickPaused(bool TickPausedValue);

};

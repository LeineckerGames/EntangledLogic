#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EntangledLogic/Interfaces/SavableInterface.h"
#include "EntangledLogic/Core/Framework/PersistantStats.h"
#include "FactorySubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnFactoryTick)

enum class EUnlockables : uint8;
UCLASS()
class ENTANGLEDLOGIC_API UFactorySubsystem : public UWorldSubsystem, public FTickableGameObject, public ISavableInterface
{
	GENERATED_BODY()
	
protected:

	bool CanTick = false;
	
	bool isTickPaused = false;

	FTimerManager TimerManager;

	TMap<EUnlockables, bool> UnlockablesMap;

public:

	class UProgressionGoals* ProgressionGoalsData;

	void TestProgressionLog();

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	void SetTickTrue();

	void SetCanTick(bool CanTickValue);

	bool GetTickPaused() const;

	TMap<EUnlockables, bool> GetUnlockablesMap() const;

	FPersistantStats PersistantStats;

	FOnFactoryTick OnFactoryTick;

	bool CheckIfUnlocked(EUnlockables UnlockToCheck);

	void UnlockProgression(EUnlockables ProgressionToUnlock);

	void RepopulateFactorySelectionWidget();

	void SetTickPaused(bool TickPausedValue);

	virtual void SaveData(class UFactorySaveGame* SaveGame) override;

	virtual void LoadData(UFactorySaveGame* SaveGame) override;

};

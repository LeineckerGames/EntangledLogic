

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SavingLoadingSubsystem.generated.h"


UCLASS()
class ENTANGLEDLOGIC_API USavingLoadingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:

	class UFactorySaveGame* FactorySaveRef;
	
	FString SaveGameSlotName = "Player";

	TArray<UObject*> RegisteredUObjects;

	void SaveGame();

	void LoadSave();

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void CreateSave();

	void RequestSave();

	void RegisterUObjectToSavingLoading(UObject* ObjectToRegister);
};

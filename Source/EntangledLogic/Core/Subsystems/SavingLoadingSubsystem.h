

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SavingLoadingSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FRequestToLoadAll)

UCLASS()
class ENTANGLEDLOGIC_API USavingLoadingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:

	UPROPERTY()
	class UFactorySaveGame* FactorySaveRef;
	
	FString SaveGameSlotName = "Player";

	UPROPERTY()
	TArray<UObject*> RegisteredUObjects;

	void SaveGame();

	void LoadSave();

	void OnLoadGameFinished(const FString& SlotName, const int32 UserIndex, class USaveGame* LoadedSave);

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void CreateSave();

	void RequestSave();

	void RequestLoad();

	void DeleteSaveFile() const;

	void RegisterUObjectToSavingLoading(UObject* ObjectToRegister);

	FRequestToLoadAll RequestToLoadAll;

};



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

	class UFactorySaveGame* FactorySaveRef;
	
	FString SaveGameSlotName = "Player";

	TArray<UObject*> RegisteredUObjects;

	void SaveGame();

	bool LoadSave();

	void OnLoadGameFinished(const FString& SlotName, const int32 UserIndex, class USaveGame* LoadedSave);

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void CreateSave();

	void RequestSave();

	void RequestLoad(UObject* Requester);

	void RegisterUObjectToSavingLoading(UObject* ObjectToRegister);

	FRequestToLoadAll RequestToLoadAll;

};

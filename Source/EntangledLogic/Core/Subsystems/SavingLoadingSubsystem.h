

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EntangledLogic/Core/Framework/SaveMetadata.h"
#include "SavingLoadingSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FRequestToLoadAll, int32 SlotIndex)
DECLARE_MULTICAST_DELEGATE(FOnSaveDirectoryUpdated)
DECLARE_MULTICAST_DELEGATE(FOnLoadFinished)

UCLASS()
class ENTANGLEDLOGIC_API USavingLoadingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:

	UPROPERTY()
	class UFactorySaveGame* FactorySaveRef;

	UPROPERTY()
	TArray<UObject*> RegisteredUObjects;

	void SaveGame(const FString& SlotName);
	void LoadSave(const FString& SlotName);
	void UpdateSlotMetadata(int32 SlotIndex, const FString& SlotName);

	void OnLoadGameFinished(const FString& SlotName, const int32 UserIndex, class USaveGame* LoadedSave);

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void CreateSave();
	void RequestSave(int32 SlotIndex, const FString& CustomSlotName);
	void RequestLoad(int32 SlotIndex);
	void DeleteSaveFile(int32 SlotIndex);

	void RegisterUObjectToSavingLoading(UObject* ObjectToRegister);

	// Helper function to get the save slot name based on the index
	FString GetSlotNameFromIndex(int32 SlotIndex) const;

	// Helper function to get the save slot metadata based on the index
	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	FSlotMetadata GetMetadataForSlot(int32 SlotIndex);

	FRequestToLoadAll RequestToLoadAll;

	FOnLoadFinished OnLoadFinished;

	FOnSaveDirectoryUpdated OnSaveDirectoryUpdated;
};

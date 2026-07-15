#include "SavingLoadingSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectIterator.h"
#include "EntangledLogic/Core/Framework/FactorySaveGame.h"
#include "EntangledLogic/Interfaces/SavableInterface.h"

void USavingLoadingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	CreateSave();
	RequestToLoadAll.AddUObject(this, &USavingLoadingSubsystem::RequestLoad);
}

void USavingLoadingSubsystem::CreateSave()
{
	if (FactorySaveRef == nullptr)
	{
		UFactorySaveGame* NewSave = Cast<UFactorySaveGame>(UGameplayStatics::CreateSaveGameObject(UFactorySaveGame::StaticClass()));
		FactorySaveRef = NewSave;
	}
}

// Helper function to get the save slot name based on the index
FString USavingLoadingSubsystem::GetSlotNameFromIndex(int32 SlotIndex) const
{
	return FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);
}

// Helper function to get the save slot metadata based on the index
FSlotMetadata USavingLoadingSubsystem::GetMetadataForSlot(int32 SlotIndex)
{
	FSlotMetadata DefaultMetadata;

	if (UGameplayStatics::DoesSaveGameExist(TEXT("SlotMetadataMaster"), 0))
	{
		if (USaveMetadata* MasterMeta = Cast<USaveMetadata>(UGameplayStatics::LoadGameFromSlot(TEXT("SlotMetadataMaster"), 0)))
		{
			if (MasterMeta->SlotRegistry.Contains(SlotIndex))
			{
				return MasterMeta->SlotRegistry[SlotIndex];
			}
		}
	}

	// Return empty default data if not found
	return DefaultMetadata;
}

// Request load for a specific slot index
void USavingLoadingSubsystem::RequestLoad(int32 SlotIndex)
{
	UE_LOG(LogTemp, Display, TEXT("Load Requested for Slot %d"), SlotIndex);
	LoadSave(GetSlotNameFromIndex(SlotIndex));
}

// Load save for a specific slot name
void USavingLoadingSubsystem::LoadSave(const FString& SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		FAsyncLoadGameFromSlotDelegate LoadDelegate;
		LoadDelegate.BindUObject(this, &USavingLoadingSubsystem::OnLoadGameFinished);

		UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadDelegate);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save file %s does not exist!"), *SlotName);
	}
}

void USavingLoadingSubsystem::OnLoadGameFinished(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedSave)
{
	if (LoadedSave)
	{
		UFactorySaveGame* LoadedFactorySaveGame = Cast<UFactorySaveGame>(LoadedSave);
		if (LoadedFactorySaveGame)
		{
			FactorySaveRef = LoadedFactorySaveGame;
			FactorySaveRef->LoadAllData(FactorySaveRef, RegisteredUObjects);
			OnLoadFinished.Broadcast();
		}
	}
}

// Request save for a specific slot index
void USavingLoadingSubsystem::RequestSave(int32 SlotIndex, const FString& CustomSlotName)
{
	if (FactorySaveRef)
	{
		UE_LOG(LogTemp, Display, TEXT("Save All Data Called"));
		FactorySaveRef->SaveAllData(FactorySaveRef, RegisteredUObjects);

		FString TargetSlotName = GetSlotNameFromIndex(SlotIndex);
		UE_LOG(LogTemp, Display, TEXT("Saving Game To: %s"), *TargetSlotName);
		SaveGame(GetSlotNameFromIndex(SlotIndex));
		UpdateSlotMetadata(SlotIndex, CustomSlotName);
	}
}

//	Save game to a specific slot name
void USavingLoadingSubsystem::SaveGame(const FString& SlotName)
{
	if (FactorySaveRef)
	{
		UGameplayStatics::AsyncSaveGameToSlot(FactorySaveRef, SlotName, 0);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FactorySaveGame Ref is NULL"));
	}
}

// Update the metadata for a specific slot index
void USavingLoadingSubsystem::UpdateSlotMetadata(int32 SlotIndex, const FString& CustomSlotName)
{
	USaveMetadata* MasterMeta = nullptr;

	// Load the master metadata save game if it exists, otherwise create a new one
	if (UGameplayStatics::DoesSaveGameExist(TEXT("SlotMetadataMaster"), 0))
	{
		MasterMeta = Cast<USaveMetadata>(UGameplayStatics::LoadGameFromSlot(TEXT("SlotMetadataMaster"), 0));
	}

	if (!MasterMeta)
	{
		MasterMeta = Cast<USaveMetadata>(UGameplayStatics::CreateSaveGameObject(USaveMetadata::StaticClass()));
	}

	if (MasterMeta)
	{
		FSlotMetadata NewMeta;
		NewMeta.bIsSlotEmpty = false;
		NewMeta.PlayerName = CustomSlotName.IsEmpty() ? FString::Printf(TEXT("Save Slot %d"), SlotIndex) : CustomSlotName;
		NewMeta.DateSaved = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M"));

		MasterMeta->SlotRegistry.Add(SlotIndex, NewMeta);
		UGameplayStatics::SaveGameToSlot(MasterMeta, TEXT("SlotMetadataMaster"), 0);

		OnSaveDirectoryUpdated.Broadcast();
	}
}

void USavingLoadingSubsystem::RegisterUObjectToSavingLoading(UObject* ObjectToRegister)
{
	if (ObjectToRegister->Implements<USavableInterface>())
	{
		RegisteredUObjects.Add(ObjectToRegister);
	}
}

// Delete save file for a specific slot index
void USavingLoadingSubsystem::DeleteSaveFile(int32 SlotIndex)
{
	// Get the slot name based on the index
	FString TargetSlotName = GetSlotNameFromIndex(SlotIndex);
	if (UGameplayStatics::DoesSaveGameExist(TargetSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(TargetSlotName, 0);
	}

	// Update the master metadata to reflect that this slot is now empty
	if (UGameplayStatics::DoesSaveGameExist(TEXT("SlotMetadataMaster"), 0))
	{
		if (USaveMetadata* MasterMeta = Cast<USaveMetadata>(UGameplayStatics::LoadGameFromSlot(TEXT("SlotMetadataMaster"), 0)))
		{
			if (MasterMeta->SlotRegistry.Contains(SlotIndex))
			{
				MasterMeta->SlotRegistry.Remove(SlotIndex);
				UGameplayStatics::SaveGameToSlot(MasterMeta, TEXT("SlotMetadataMaster"), 0);
				OnSaveDirectoryUpdated.Broadcast();
			}
		}
	}
}
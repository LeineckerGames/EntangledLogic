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

void USavingLoadingSubsystem::RequestLoad()
{
	UE_LOG(LogTemp, Display, TEXT("Load Requested"));
	LoadSave();
}

void USavingLoadingSubsystem::LoadSave()
{
	bool DoesSaveGameExist = UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, 0);
	if(DoesSaveGameExist)
	{
		FAsyncLoadGameFromSlotDelegate LoadDelegate;
		LoadDelegate.BindUObject(this, &USavingLoadingSubsystem::OnLoadGameFinished);

		UGameplayStatics::AsyncLoadGameFromSlot(SaveGameSlotName, 0, LoadDelegate);
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
		}
	}
}

void USavingLoadingSubsystem::RequestSave()
{
	if (FactorySaveRef)
	{
		UE_LOG(LogTemp, Display, TEXT("Save All Data Called"));
		FactorySaveRef->SaveAllData(FactorySaveRef, RegisteredUObjects);
		UE_LOG(LogTemp, Display, TEXT("Save Game Called"));
		SaveGame();
	}
}

void USavingLoadingSubsystem::SaveGame()
{
	if (FactorySaveRef)
	{
		UGameplayStatics::AsyncSaveGameToSlot(FactorySaveRef, SaveGameSlotName, 0);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("FactorySaveGame Ref is NULL"));
	}
	
}

void USavingLoadingSubsystem::RegisterUObjectToSavingLoading(UObject* ObjectToRegister)
{
	if (ObjectToRegister->Implements<USavableInterface>())
	{
		RegisteredUObjects.Add(ObjectToRegister);
	}
}

void USavingLoadingSubsystem::DeleteSaveFile() const
{
	UGameplayStatics::DeleteGameInSlot(SaveGameSlotName, 0);
}
//LogTemp: Display: Save Game Pressed
//LogTemp : Display: Save All Data Called
//LogTemp : Display: Save Game Called
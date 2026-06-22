#include "SavingLoadingSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectIterator.h"
#include "EntangledLogic/Core/Framework/FactorySaveGame.h"
#include "EntangledLogic/Interfaces/SavableInterface.h"

void USavingLoadingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	CreateSave();
}

void USavingLoadingSubsystem::CreateSave()
{
	if (FactorySaveRef == nullptr)
	{
		UFactorySaveGame* NewSave = Cast<UFactorySaveGame>(UGameplayStatics::CreateSaveGameObject(UFactorySaveGame::StaticClass()));
		FactorySaveRef = NewSave;
	}
}

void USavingLoadingSubsystem::RequestLoad(UObject* Requester)
{
	FactorySaveRef->LoadRequested(FactorySaveRef, Requester);
}

bool USavingLoadingSubsystem::LoadSave()
{
	bool DoesSaveGameExist = UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, 0);
	if(DoesSaveGameExist)
	{
		FAsyncLoadGameFromSlotDelegate LoadDelegate;
		LoadDelegate.BindUObject(this, &USavingLoadingSubsystem::OnLoadGameFinished);

		UGameplayStatics::AsyncLoadGameFromSlot(SaveGameSlotName, 0, LoadDelegate);
		return true;
	}
	return false;
}

void USavingLoadingSubsystem::OnLoadGameFinished(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedSave)
{
	if (LoadedSave)
	{
		UFactorySaveGame* LoadedFactorySaveGame = Cast<UFactorySaveGame>(LoadedSave);
		if (LoadedFactorySaveGame)
		{
			FactorySaveRef = LoadedFactorySaveGame;
		}
	}
}

void USavingLoadingSubsystem::RequestSave()
{
	UE_LOG(LogTemp, Display, TEXT("Save All Data Called"));
	FactorySaveRef->SaveAllData(FactorySaveRef, RegisteredUObjects);
	UE_LOG(LogTemp, Display, TEXT("Save Game Called"));
	SaveGame();
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
//LogTemp: Display: Save Game Pressed
//LogTemp : Display: Save All Data Called
//LogTemp : Display: Save Game Called
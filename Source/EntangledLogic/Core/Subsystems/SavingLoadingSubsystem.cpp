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

void USavingLoadingSubsystem::LoadSave()
{

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
	UGameplayStatics::AsyncSaveGameToSlot(FactorySaveRef, SaveGameSlotName, 0);
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
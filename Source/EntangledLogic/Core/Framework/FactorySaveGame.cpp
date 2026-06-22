#include "FactorySaveGame.h"
#include "EntangledLogic/Interfaces/SavableInterface.h"
#include "Kismet/GameplayStatics.h"

void UFactorySaveGame::SaveAllData(UFactorySaveGame* SaveGame, TArray<UObject*> SavableObjects)
{
	for (UObject* CurrentObject : SavableObjects)
	{
		if (CurrentObject)
		{
			ISavableInterface* SavableObject = Cast<ISavableInterface>(CurrentObject);
			SavableObject->SaveData(this);
		}
	}
}


void UFactorySaveGame::LoadAllData(UFactorySaveGame* SaveGame, TArray<UObject*> SavableObjects)
{
	for (UObject* CurrentObject : SavableObjects)
	{
		if (CurrentObject)
		{
			ISavableInterface* SavableObject = Cast<ISavableInterface>(CurrentObject);
			SavableObject->LoadData(this);
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Data Loaded"));
}

// Should be able to remove once done
void UFactorySaveGame::LoadRequested(UFactorySaveGame* SaveGame, UObject* Requester)
{
	ISavableInterface* SavableObject = Cast<ISavableInterface>(Requester);
	if (SavableObject)
	{
		SavableObject->LoadData(this);
	}
}

#include "FactorySaveGame.h"
#include "EntangledLogic/Interfaces/SavableInterface.h"
#include "Kismet/GameplayStatics.h"

void UFactorySaveGame::SaveAllData(UFactorySaveGame* SaveGame, TArray<UObject*> SavableObjects)
{
	for (UObject* CurrentObject : SavableObjects)
	{
		ISavableInterface* SavableObject = Cast<ISavableInterface>(CurrentObject);
		SavableObject->SaveData(this);
	}
}
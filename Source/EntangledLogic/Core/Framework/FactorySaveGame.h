#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EntangledLogic/Core/Framework/PersistantStats.h"
#include "FactorySaveGame.generated.h"

struct FPersistantStats;
UCLASS()
class ENTANGLEDLOGIC_API UFactorySaveGame : public USaveGame
{
	GENERATED_BODY()
	
protected:


public:
	void SaveAllData(UFactorySaveGame* SaveGame, TArray<UObject*> SavableObjects);

	void LoadRequested(UFactorySaveGame* SaveGame, UObject* Requester);

	FPersistantStats PersistantStats;
};

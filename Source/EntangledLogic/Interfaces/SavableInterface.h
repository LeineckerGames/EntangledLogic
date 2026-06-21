#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableInterface.generated.h"

class UFactorySaveGame;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USavableInterface : public UInterface
{
	GENERATED_BODY()
};

class ENTANGLEDLOGIC_API ISavableInterface
{
	GENERATED_BODY()

public:

	virtual void SaveData(UFactorySaveGame* SaveGame) = 0;

	virtual void LoadData(UFactorySaveGame* SaveGame) = 0;

};

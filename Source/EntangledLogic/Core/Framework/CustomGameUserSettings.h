#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CustomGameUserSettings.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API UCustomGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

	// This entire file is redundant, but it is here to allow for
	// future customization of the GameUserSettings class if needed (Settings Menu->Gameplay).
};
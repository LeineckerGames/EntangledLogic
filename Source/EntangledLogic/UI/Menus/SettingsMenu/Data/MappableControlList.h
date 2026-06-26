#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/Data/SettingsDataTypes.h"
#include "MappableControlList.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API UMappableControlList : public UDataAsset
{
	GENERATED_BODY()
	
	public:
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controls Configuration")
		TArray<FKeyMappingDisplayInfo> ControlRows;
};
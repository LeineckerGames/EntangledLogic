#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EntangledLogic/Core/Framework/ProgressionGoalsDataStructs.h"
#include "ProgressionGoalsDataAsset.generated.h"

UCLASS(BlueprintType)
class ENTANGLEDLOGIC_API UProgressionGoalsDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TMap<EProgressionGoals, FProgressionGoalsData> ProgressionGoals;

};

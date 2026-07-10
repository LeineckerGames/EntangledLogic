#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProgressionGoals.generated.h"


UENUM()
enum class EProgressionGoals : uint8
{
	Ket_One_State,
	Negative_Ket_One_State,
	Superpositon_State
};

USTRUCT(BlueprintType)
struct FProgressionGoalsData
{
	GENERATED_BODY()

	// Qpp::Ket AcceptedState

	int32 RequiredStatesAmount;
};


UCLASS()
class ENTANGLEDLOGIC_API UProgressionGoals : public UDataAsset
{
	GENERATED_BODY()
	
};

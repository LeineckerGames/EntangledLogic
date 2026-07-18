#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/StaticMesh.h"
#include "EntangledLogic/Core/Framework/ProgressionGoalsDataAsset.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"
#include "FactorySettings.generated.h"

/**
 * Global factory settings for the project.
 * Found in Edit -> Project Settings -> Game -> Factory Settings
 */
UCLASS(Config=Game, defaultconfig, meta=(DisplayName="Factory Settings"))
class ENTANGLEDLOGIC_API UFactorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// The default mesh to use for a Qubit traveling along a wire
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Wire Settings")
	TSoftObjectPtr<UStaticMesh> QubitMesh;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Progression Data")
	//UProgressionGoals* ProgressionGoalsDataAsset;
};
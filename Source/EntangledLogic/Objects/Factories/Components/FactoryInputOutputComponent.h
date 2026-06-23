#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FactoryInputOutputComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENTANGLEDLOGIC_API UFactoryInputOutputComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UFactoryInputOutputComponent();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMeshComponent*> InputOutputMeshes;

public:	

	void SetMeshesVisible(bool isVisible);
		
};

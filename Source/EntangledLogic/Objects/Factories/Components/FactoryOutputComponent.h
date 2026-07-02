#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FactoryOutputComponent.generated.h"

UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class ENTANGLEDLOGIC_API UFactoryOutputComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UFactoryOutputComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* OutputMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector OutputMeshLocationOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator OutputMeshRotationOffset = FRotator(0.0f, 90.0f, 0.0f);

public:	
	void SetMeshVisibility(bool isVisible);

	UPROPERTY(EditAnywhere)
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* OutputSlot;

	UStaticMeshComponent* GetMesh();

};

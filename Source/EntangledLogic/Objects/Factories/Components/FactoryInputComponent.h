#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FactoryInputComponent.generated.h"

UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class ENTANGLEDLOGIC_API UFactoryInputComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UFactoryInputComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* InputMesh;

public:	
	void SetMeshVisibility(bool isVisible);

	UPROPERTY(EditAnywhere)
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* InputSlot;

	UStaticMeshComponent* GetMesh();

};

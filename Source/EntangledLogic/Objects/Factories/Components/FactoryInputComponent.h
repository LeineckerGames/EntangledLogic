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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector InputMeshLocationOffset = FVector(-100.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator InputMeshRotationOffset = FRotator(0.0f, 90.0f, 0.0f);

public:	
	void SetMeshVisibility(bool isVisible);

	UPROPERTY(EditAnywhere)
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* InputSlot;

	UStaticMeshComponent* GetMesh();

};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntangledLogic/Interfaces/FactoryInteractionInterface.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "FactoryBase.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API AFactoryBase : public AActor, public IFactoryInteractionInterface, public IInputOutputInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFactoryBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere)
	class UGridPlacementComponent* GridPlacementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* FactoryMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFactoryInputOutputComponent* InputOutputComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* FactoryDisplayWindow;

	class UUserWidget* FactoryWidget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RotateUIToCamera();

	// Factory Interacton Interface
	virtual void BeginHover(EPlacementMode PlacementMode) override;
	virtual void EndHover(EPlacementMode PlacementMode) override;

	virtual void Interact(EPlacementMode PlacementMode) override;

	// Input Output Interface
	virtual void ConnectAllInputsAndOutputs() override;

	virtual void ConnectAllInputs() override;

	virtual void ConnectAllOutputs() override;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntangledLogic/Interfaces/FactoryInteractionInterface.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "Containers/CircularQueue.h"
#include "Wire.generated.h"

class AQubit;

UCLASS()
class ENTANGLEDLOGIC_API AWire : public AActor, public IFactoryInteractionInterface, public IInputOutputInterface
{
	GENERATED_BODY()
	

	// MEMBERS ARE COPIED FROM AFactoryBase
	static constexpr int32 NUM_QUBIT_SLOTS = 5;

public:	
	// Sets default values for this actor's properties
	AWire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere)
	class UGridPlacementComponent* GridPlacementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* WireMesh;

	UPROPERTY(VisibleAnywhere, Category = "InputOutputs")
	TArray<class UFactoryInputComponent*> InputComponents;

	UPROPERTY(VisibleAnywhere, Category = "InputOutputs")
	TArray<class UFactoryOutputComponent*> OutputComponents;

	void OnLoadCompleted();

	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* FactoryDisplayWindow;
	*/


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//TCircularQueue<AQubit*> Qubits{ NUM_QUBIT_SLOTS };

	// Factory Interacton Interface
	virtual void BeginHover(EPlacementMode PlacementMode) override;
	virtual void EndHover(EPlacementMode PlacementMode) override;

	virtual void Interact(EPlacementMode PlacementMode) override;

	// Input Output Interface
	virtual void SetAllInputOutputsVisibility(bool isVisible) override;

	virtual TArray<class UFactoryInputComponent*> GetInputComponents() override;

	virtual TArray<class UFactoryOutputComponent*> GetOutputComponents() override;

	virtual void ConnectAllInputsAndOutputs() override;

	//virtual bool IsQubitSlotEmpty(int32 QubitSlotIndex) override;

	//virtual void TransferQubit(class AQubit* QubitToTransfer, int32 QubitSlotIndex);

	virtual void ConnectAllInputs() override;

	virtual void ConnectAllOutputs() override;
};

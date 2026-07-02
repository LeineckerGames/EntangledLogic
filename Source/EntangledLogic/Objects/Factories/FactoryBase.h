#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntangledLogic/Interfaces/FactoryInteractionInterface.h"
#include "FactoryBase.generated.h"

class AQubit;

UCLASS()
class ENTANGLEDLOGIC_API AFactoryBase : public AActor, public IFactoryInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFactoryBase();

	static constexpr int32 NUM_QUBIT_SLOTS = 0;

	TArray<AQubit*> Qubits;

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

	// this will apparently correctly allow the constant to be overriden by subclasses
	virtual int32 GetNumQubitSlots() { return NUM_QUBIT_SLOTS; };

	void RotateUIToCamera();

	void UpdateQubitDisplay();

	// Factory Interacton Interface
	virtual void BeginHover(EPlacementMode PlacementMode) override;
	virtual void EndHover(EPlacementMode PlacementMode) override;

	virtual void Interact(EPlacementMode PlacementMode) override;
};

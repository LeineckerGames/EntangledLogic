

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GridPlacementComponent.generated.h"

enum class EPlacementMode : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENTANGLEDLOGIC_API UGridPlacementComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridPlacementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Factory Shape Info
	UPROPERTY(EditAnywhere, Category = "Factory Shape")
	int32 FactorySize;
	UPROPERTY(EditAnywhere, Category = "Factory Shape")
	TArray<bool> FactoryShape;
	UPROPERTY(EditAnywhere, Category = "Factory Shape")
	FVector PlacementOffset;

	float GridSize;

	// Materials
	UPROPERTY(EditAnywhere, Category = "Grid Materials")
	class UMaterialInterface* FactoryCollisionOverlayMaterial;

	//// Might combine this into above material
	//UPROPERTY(EditAnywhere, Category = "Grid Materials")
	//class UMaterialInterface* FactoryOutlineOverlayMaterial;

	class UMaterialInstanceDynamic* OverlayMaterial;

	TArray<UMeshComponent*> ActorsAttachedMeshes;

	void UpdateOverlayMaterial(TArray<UMeshComponent*> MeshesToUpdate);

	void UpdateRenderCustomDepth(TArray<UMeshComponent*> MeshesToUpdate, bool value);

	void UpdateCustomDepthStencilValue(TArray<UMeshComponent*> MeshesToUpdate, int32 value);

	void OnPlacementModeChanged(EPlacementMode CurrentPlacementMode);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FVector GetPlacementOffset() const;
	
	int32 GetFactorySize() const;
	
	TArray<bool> GetFactoryShape() const;
	
	TArray<UMeshComponent*> GetActorsAttachedMeshes() const;

	void RemoveOverlayMaterial();

	void UpdateCollisionMaterialParam(bool CollisionPass);

	void EnableEditOutline();

	void EnableDeleteOutline();

	void DisableOutline();

};

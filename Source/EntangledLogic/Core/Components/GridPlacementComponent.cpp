#include "GridPlacementComponent.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"

UGridPlacementComponent::UGridPlacementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGridPlacementComponent::BeginPlay()
{
	Super::BeginPlay();

	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	GridSize = GridPlacement->GetGridSize();

	// Gets all the meshes attached to the parent actor
	GetOwner()->GetComponents<UMeshComponent>(ActorsAttachedMeshes);

	// Creates Dynamic material instance
	if (FactoryCollisionOverlayMaterial)
	{
		OverlayMaterial = UMaterialInstanceDynamic::Create(FactoryCollisionOverlayMaterial, this);
		UpdateOverlayMaterial(ActorsAttachedMeshes);
	}

	// Sets Render Custom Depth to true so we can use the outline material
	UpdateRenderCustomDepth(ActorsAttachedMeshes);
	
}


// Called every frame
void UGridPlacementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGridPlacementComponent::UpdateOverlayMaterial(TArray<UMeshComponent*> MeshesToUpdate)
{
	if (OverlayMaterial)
	{
		for (UMeshComponent* Mesh : MeshesToUpdate)
		{
			Mesh->SetOverlayMaterial(OverlayMaterial);
		}
	}
}

void UGridPlacementComponent::UpdateRenderCustomDepth(TArray<UMeshComponent*> MeshesToUpdate)
{
		for (UMeshComponent* Mesh : MeshesToUpdate)
		{
			Mesh->SetRenderCustomDepth(true);
		}
}

void UGridPlacementComponent::RemoveOverlayMaterial()
{
		for (UMeshComponent* Mesh : ActorsAttachedMeshes)
		{
			Mesh->SetOverlayMaterial(nullptr);
		}
}

void UGridPlacementComponent::UpdateCollisionMaterialParam(bool CollisionPass)
{
	OverlayMaterial->SetScalarParameterValue(FName("CollisionPass"), CollisionPass);
}

// Getters

FVector UGridPlacementComponent::GetPlacementOffset() const
{
	return PlacementOffset;
}

int32 UGridPlacementComponent::GetFactorySize() const
{
	return FactorySize;
}

TArray<bool> UGridPlacementComponent::GetFactoryShape() const
{
	return FactoryShape;
}

TArray<UMeshComponent*> UGridPlacementComponent::GetActorsAttachedMeshes() const
{
	return ActorsAttachedMeshes;
}
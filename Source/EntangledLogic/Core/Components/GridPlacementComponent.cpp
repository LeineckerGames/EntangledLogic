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
	GridPlacement->OnPlacementModeChanged.AddUObject(this, &UGridPlacementComponent::OnPlacementModeChanged);


	// Gets all the meshes attached to the parent actor
	GetOwner()->GetComponents<UMeshComponent>(ActorsAttachedMeshes);

	// Creates Dynamic material instance
	if (FactoryCollisionOverlayMaterial)
	{
		CollisionOverlayMaterial = UMaterialInstanceDynamic::Create(FactoryCollisionOverlayMaterial, this);
		UpdateOverlayMaterial(ActorsAttachedMeshes, CollisionOverlayMaterial);
	}

	UpdateRenderCustomDepth(ActorsAttachedMeshes, false);
	
}


// Called every frame
void UGridPlacementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGridPlacementComponent::OnPlacementModeChanged(EPlacementMode CurrentPlacementMode)
{
	UE_LOG(LogTemp, Display, TEXT("Placement changed in grid comp"));
	switch (CurrentPlacementMode)
	{
	case EPlacementMode::Disabled:
		// can make this handle the remove overlays now
		RemoveOverlayMaterial();
		UpdateRenderCustomDepth(ActorsAttachedMeshes, false);
		break;
	case EPlacementMode::Placing:
		UpdateRenderCustomDepth(ActorsAttachedMeshes, false);
		break;
	case EPlacementMode::Editing:
		RemoveOverlayMaterial();
		// Enable Outline & set to green
		// need to change to be on hover not all meshes next
		UpdateRenderCustomDepth(ActorsAttachedMeshes, true);
		UpdateCustomDepthStencilValue(ActorsAttachedMeshes, 2);
		break;
	case EPlacementMode::Deletion:
		RemoveOverlayMaterial();
		// Enable Outline & set to red
		UpdateRenderCustomDepth(ActorsAttachedMeshes, true);
		UpdateCustomDepthStencilValue(ActorsAttachedMeshes, 1);
		break;
	}
}

void UGridPlacementComponent::UpdateOverlayMaterial(TArray<UMeshComponent*> MeshesToUpdate, UMaterialInstanceDynamic* Material)
{
	if (Material)
	{
		for (UMeshComponent* Mesh : MeshesToUpdate)
		{
			Mesh->SetOverlayMaterial(Material);
		}
	}
}

void UGridPlacementComponent::UpdateRenderCustomDepth(TArray<UMeshComponent*> MeshesToUpdate, bool value)
{
		for (UMeshComponent* Mesh : MeshesToUpdate)
		{
			Mesh->SetRenderCustomDepth(value);
		}
}

// 1 For Delete, 2 For Edit
void UGridPlacementComponent::UpdateCustomDepthStencilValue(TArray<UMeshComponent*> MeshesToUpdate, int32 value)
{
		for (UMeshComponent* Mesh : MeshesToUpdate)
		{
			Mesh->SetCustomDepthStencilValue(value);
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
	CollisionOverlayMaterial->SetScalarParameterValue(FName("CollisionPass"), CollisionPass);
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
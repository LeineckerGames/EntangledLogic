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
		OverlayMaterial = UMaterialInstanceDynamic::Create(FactoryCollisionOverlayMaterial, this);
		UpdateOverlayMaterial(ActorsAttachedMeshes);
	}

	UpdateRenderCustomDepth(ActorsAttachedMeshes, false);
	
}

void UGridPlacementComponent::OnPlacementModeChanged(EPlacementMode CurrentPlacementMode)
{
	//UE_LOG(LogTemp, Display, TEXT("Placement changed in grid comp"));
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
		UpdateRenderCustomDepth(ActorsAttachedMeshes, false);
		break;
	case EPlacementMode::Deletion:
		RemoveOverlayMaterial();
		UpdateRenderCustomDepth(ActorsAttachedMeshes, false);
		break;
	}
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
			// Skips any meshes marked not to show overlay
			if (!Mesh->ComponentHasTag(FName("DontShowCollision")))
			{
				Mesh->SetOverlayMaterial(OverlayMaterial);
			}
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

// 1 for Delete, 2 For Edit, 3 for rainbow
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
	if (OverlayMaterial)
	{
		OverlayMaterial->SetScalarParameterValue(FName("CollisionPass"), CollisionPass);
	}
}

void UGridPlacementComponent::EnableEditOutline()
{
	UpdateRenderCustomDepth(ActorsAttachedMeshes, true);
	UpdateCustomDepthStencilValue(ActorsAttachedMeshes, 2);
}

void UGridPlacementComponent::EnableDeleteOutline()
{
	UpdateRenderCustomDepth(ActorsAttachedMeshes, true);
	UpdateCustomDepthStencilValue(ActorsAttachedMeshes, 1);
}

void UGridPlacementComponent::EnableInteractionOutline()
{
	UpdateRenderCustomDepth(ActorsAttachedMeshes, true);
	UpdateCustomDepthStencilValue(ActorsAttachedMeshes, 3);
}

void UGridPlacementComponent::DisableOutline()
{
	UpdateRenderCustomDepth(ActorsAttachedMeshes, false);
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
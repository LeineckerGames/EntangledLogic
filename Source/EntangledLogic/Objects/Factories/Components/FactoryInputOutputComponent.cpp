#include "FactoryInputOutputComponent.h"

UFactoryInputOutputComponent::UFactoryInputOutputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFactoryInputOutputComponent::BeginPlay()
{
	Super::BeginPlay();

	// Attches Meshes to I/O Comonenet so they move with the factory
	TArray<UStaticMeshComponent*> InputOutputMeshes;
	InputOutputMeshes.Append(InputMeshes);
	InputOutputMeshes.Append(OutputMeshes);
	for (UStaticMeshComponent* CurrentMesh : InputOutputMeshes)
	{
		if (CurrentMesh)
		{
			CurrentMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void UFactoryInputOutputComponent::SetMeshesVisible(bool isVisible)
{
	TArray<UStaticMeshComponent*> InputOutputMeshes;
	InputOutputMeshes.Append(InputMeshes);
	InputOutputMeshes.Append(OutputMeshes);
	for (UStaticMeshComponent* CurrentMesh : InputOutputMeshes)
	{
		if (CurrentMesh)
		{
			CurrentMesh->SetVisibility(isVisible);
		}
	}
}

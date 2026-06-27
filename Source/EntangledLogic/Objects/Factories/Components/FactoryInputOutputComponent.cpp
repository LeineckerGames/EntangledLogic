#include "FactoryInputOutputComponent.h"

UFactoryInputOutputComponent::UFactoryInputOutputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFactoryInputOutputComponent::BeginPlay()
{
	Super::BeginPlay();
	TArray<UStaticMeshComponent*> InputOutputMeshes;
	InputOutputMeshes.Append(InputMeshes);
	InputOutputMeshes.Append(OutputMeshes);
	// Attches Meshes to I/O Comonenet so they move with the factory
	for (UStaticMeshComponent* CurrentMesh : InputOutputMeshes)
	{
		CurrentMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void UFactoryInputOutputComponent::SetMeshesVisible(bool isVisible)
{
	TArray<UStaticMeshComponent*> InputOutputMeshes;
	InputOutputMeshes.Append(InputMeshes);
	InputOutputMeshes.Append(OutputMeshes);
	for (UStaticMeshComponent* CurrentMesh : InputOutputMeshes)
	{
		CurrentMesh->SetVisibility(isVisible);
	}
}

TArray<UStaticMeshComponent*> UFactoryInputOutputComponent::GetInputMeshes()
{
	return InputMeshes;
}

TArray<UStaticMeshComponent*> UFactoryInputOutputComponent::GetOutputMeshes()
{
	return OutputMeshes;
}
#include "FactoryInputOutputComponent.h"

UFactoryInputOutputComponent::UFactoryInputOutputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFactoryInputOutputComponent::BeginPlay()
{
	Super::BeginPlay();

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
	for (UStaticMeshComponent* CurrentMesh : InputOutputMeshes)
	{
		if (CurrentMesh)
		{
			CurrentMesh->SetVisibility(isVisible);
		}
	}
}

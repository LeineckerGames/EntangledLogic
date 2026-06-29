#include "FactoryOutputComponent.h"

UFactoryOutputComponent::UFactoryOutputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	OutputMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OutputMesh"));
	OutputMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	OutputMesh->ComponentTags.Add(FName("DontShowCollision"));
}


void UFactoryOutputComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UFactoryOutputComponent::SetMeshVisibility(bool isVisible)
{
	OutputMesh->SetVisibility(isVisible);
}

UStaticMeshComponent* UFactoryOutputComponent::GetMesh()
{
	return OutputMesh;
}
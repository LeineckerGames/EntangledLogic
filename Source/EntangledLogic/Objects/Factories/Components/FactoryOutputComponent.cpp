#include "FactoryOutputComponent.h"

UFactoryOutputComponent::UFactoryOutputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	OutputMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OutputMesh"));
	OutputMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	// Add Offsets
	OutputMesh->SetRelativeLocation(OutputMeshLocationOffset);
	OutputMesh->SetRelativeRotation(OutputMeshRotationOffset);
	OutputMesh->ComponentTags.Add(FName("DontShowCollision"));
}


void UFactoryOutputComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UFactoryOutputComponent::SetMeshVisibility(bool isVisible)
{
	OutputMesh->SetVisibility(isVisible);
	FVector ForwardVector = GetForwardVector();
	UE_LOG(LogTemp, Display, TEXT("Output Forward Vector X: %f Y: %f Z: %f"), ForwardVector.X, ForwardVector.Y, ForwardVector.Z)
}

UStaticMeshComponent* UFactoryOutputComponent::GetMesh()
{
	return OutputMesh;
}
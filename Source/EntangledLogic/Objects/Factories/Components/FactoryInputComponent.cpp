#include "FactoryInputComponent.h"


UFactoryInputComponent::UFactoryInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InputMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InputMesh"));
	InputMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	// Add Offsets
	InputMesh->SetRelativeLocation(InputMeshLocationOffset);
	InputMesh->SetRelativeRotation(InputMeshRotationOffset);
	InputMesh->ComponentTags.Add(FName("DontShowCollision"));
}


// Called when the game starts
void UFactoryInputComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UFactoryInputComponent::SetMeshVisibility(bool isVisible)
{
	InputMesh->SetVisibility(isVisible);
	FVector ForwardVector = GetForwardVector();
	UE_LOG(LogTemp, Display, TEXT("Input Forward Vector X: %f Y: %f Z: %f"), ForwardVector.X, ForwardVector.Y, ForwardVector.Z)
}


UStaticMeshComponent* UFactoryInputComponent::GetMesh()
{
	return InputMesh;
}
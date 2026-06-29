#include "FactoryInputComponent.h"


UFactoryInputComponent::UFactoryInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InputMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InputMesh"));
	InputMesh->AttachToComponent(this , FAttachmentTransformRules::KeepRelativeTransform);
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
}


UStaticMeshComponent* UFactoryInputComponent::GetMesh()
{
	return InputMesh;
}
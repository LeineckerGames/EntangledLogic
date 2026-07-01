#include "TestingWire.h"

ATestingWire::ATestingWire()
{
	// A basic TestingWire doesn't need to tick anymore as segment handles motion
	PrimaryActorTick.bCanEverTick = false;
}

void ATestingWire::BeginPlay()
{
	Super::BeginPlay();
}

void ATestingWire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



#include "TestingFactory.h"

ATestingFactory::ATestingFactory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestingFactory::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATestingFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

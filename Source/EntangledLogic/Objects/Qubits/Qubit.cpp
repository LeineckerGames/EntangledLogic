


#include "EntangledLogic/Objects/Qubits/Qubit.h"
#include "EntangledLogic/Core/Framework/QubitDataStructs.h"
#include "Templates/SharedPointer.h"

// Sets default values
AQubit::AQubit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQubit::BeginPlay()
{
	Super::BeginPlay();
	State = MakeShared<FQubitData>();
	
}

// Called every frame
void AQubit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


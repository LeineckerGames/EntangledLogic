#include "LargerTestFactory.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"

void ALargerTestFactory::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		UQubitDataSubsystem* QubitSubsystem = World->GetSubsystem<UQubitDataSubsystem>();
		if (QubitSubsystem)
		{
			FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 80);
			Qubits[0] = QubitSubsystem->SpawnQubit(SpawnLocation);
			Qubits[0]->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
			UpdateQubitDisplay();
		}
	}
}

void ALargerTestFactory::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Qubits[0])
	{
		UWorld* World = GetWorld();
		if (World)
		{
			UQubitDataSubsystem* QubitSubsystem = World->GetSubsystem<UQubitDataSubsystem>();
			if (QubitSubsystem)
			{
				QubitSubsystem->DeleteQubit(*Qubits[0]);
			}
		}
	}
}

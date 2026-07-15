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
			Qubits[0] = QubitSubsystem->NewQubit();
			Qubits[0]->SetActorRelativeLocation(FVector(0, 0, 80));
			Qubits[0]->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
			UpdateQubitDisplay();
		}
	}
}

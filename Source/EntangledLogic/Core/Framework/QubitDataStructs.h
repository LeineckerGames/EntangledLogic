#pragma once

#include "CoreMinimal.h"
#include "QppPlugin.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"
#include "Components/SplineMeshComponent.h"
#include "QubitDataStructs.generated.h"

USTRUCT()
struct FQubitData
{
	GENERATED_USTRUCT_BODY()

	//bool bIsDensityMatrix = false;
	//qpp::cmat DensityMatrix;

	//UPROPERTY()
	qpp::ket StateVector;

	UPROPERTY()
	TArray<AQubit*> qubits;

	void UpdateQubitEntanglmentSplines()
	{
		UE_LOG(LogTemp, Display, TEXT("Qubits Size = %d"), qubits.Num());
		for (AQubit* CurrentQubit : qubits)
		{
			// Prob going to need to remove all splines before
			for (AQubit* QubitToAttachTo : qubits)
			{
				if (CurrentQubit != QubitToAttachTo)
				{
					CurrentQubit->AttachEntanglementSplineToQubit(QubitToAttachTo);
					CurrentQubit->SetEntanglementSplineVisibility(false);
				}
			}
		}
	}

	void MoveEntanglementSpline()
	{
		for (AQubit* CurrentQubit : qubits)
		{
			int32 Count = 0;
			for (AQubit* QubitToAttachTo : qubits)
			{
				if (CurrentQubit != QubitToAttachTo)
				{
					FVector QubitToAttachLocation = QubitToAttachTo->GetTransform().GetLocation();
					CurrentQubit->UpdateEntanglementSplinePointLocation(QubitToAttachLocation, Count);
					Count++;
				}
			}
		}
	}

};
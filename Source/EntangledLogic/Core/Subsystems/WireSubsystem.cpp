#include "WireSubsystem.h"
#include "EntangledLogic/Objects/TestingWire.h"
#include "EntangledLogic/Objects/WireSegment.h"

void UWireSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}


void UWireSubsystem::AddWireToPaths(ATestingWire* NewWire)
{
	if (!NewWire) return;

	ATestingWire* Prev = NewWire->GetInputWire();
	ATestingWire* Next = NewWire->GetOutputWire();

	// Scenario 1: Merging two separate paths
	if (Prev && Prev->AssignedSegment && Next && Next->AssignedSegment && Prev->AssignedSegment != Next->AssignedSegment)
	{
		AWireSegment* FirstSegment = Prev->AssignedSegment;
		AWireSegment* SecondSegment = Next->AssignedSegment;

		// Merge SecondSegment into FirstSegment
		NewWire->AssignedSegment = FirstSegment;

		// Update all wires in the second segment to point to the first
		ATestingWire* CurrentWire = Next;
		while (CurrentWire)
		{
			CurrentWire->AssignedSegment = FirstSegment;
			CurrentWire = CurrentWire->GetOutputWire();
		}

		// Rebuild the spline 
		FirstSegment->InitializeSegment(FirstSegment->StartWire);

		// Remove the old second segment
		ActiveSegments.Remove(SecondSegment);
		SecondSegment->Destroy();
	}
	// Scenario 2: Extending the end of a path
	else if (Prev && Prev->AssignedSegment)
	{
		NewWire->AssignedSegment = Prev->AssignedSegment;
		Prev->AssignedSegment->AddWireToEndOfSegment(NewWire);
	}
	// Scenario 3: Extending the start of a path
	else if (Next && Next->AssignedSegment)
	{
		NewWire->AssignedSegment = Next->AssignedSegment;
		
		// The new wire is now the start of this segment
		Next->AssignedSegment->AddWireToStartOfSegment(NewWire);
	}
	// Scenario 4: A brand new, standalone wire
	else
	{
		AWireSegment* NewSegment = CreateNewSegment(NewWire);
		NewWire->AssignedSegment = NewSegment;
	}
}

void UWireSubsystem::RemoveWireFromPath(ATestingWire* RemovedWire)
{
	if (!RemovedWire || !RemovedWire->AssignedSegment) return;

	//AWireSegment* OldSegment = RemovedWire->AssignedSegment;
    ATestingWire* Prev = RemovedWire->GetInputWire();
    ATestingWire* Next = RemovedWire->GetOutputWire();

	/* // Below code is not necessary when the pointers are being disconnected elsewhere 
    // Disconnect pointers
    if (Prev) Prev->NextWire = nullptr; // alternatively Prev->SetOutputWire(nullptr) // need to make the Set function
    if (Next) Next->PreviousWire = nullptr; // alternatively Next->SetInputWire(nullptr) // need to make the Set function
	*/

	/*
	// Remove the old segment completely (in a full game, we'd transfer items first)
    ActiveSegments.Remove(OldSegment);
    OldSegment->Destroy();
	*/

	// Scenario 1: Splitting into two separate paths
	if (Prev && Prev->AssignedSegment && Next && Next->AssignedSegment)
	{
		/*
		AWireSegment* FirstSegment = Prev->AssignedSegment;
		AWireSegment* SecondSegment = Next->AssignedSegment;

		// Merge SecondSegment into FirstSegment
		RemovedWire->AssignedSegment = FirstSegment;

		// Update all wires in the second segment to point to the first
		ATestingWire* CurrentWire = Next;
		while (CurrentWire)
		{
			CurrentWire->AssignedSegment = FirstSegment;
			CurrentWire = CurrentWire->GetOutputWire();
		}

		// Rebuild the spline 
		FirstSegment->InitializeSegment(FirstSegment->StartWire);

		// Remove the old second segment
		ActiveSegments.Remove(SecondSegment);
		SecondSegment->Destroy();
		*/
	}
	// Scenario 2: Removing the end of a path
	else if (Prev && Prev->AssignedSegment)
	{
		RemovedWire->AssignedSegment->RemoveWireFromEndOfSegment(RemovedWire);
		RemovedWire->AssignedSegment = nullptr;
	}
	// Scenario 3: Removing the start of a path
	else if (Next && Next->AssignedSegment)
	{
		Next->AssignedSegment->RemoveWireFromStartOfSegment(RemovedWire);
		RemovedWire->AssignedSegment = nullptr;
	}
	// Scenario 4: A standalone wire
	else
	{
		/*
		AWireSegment* NewSegment = CreateNewSegment(RemovedWire);
		RemovedWire->AssignedSegment = NewSegment;
		*/
	}

	/*
    // Recalculate the separated pieces
    if (Prev)
    {
        // Re-add the previous part, tracing backwards to find its new start
        ATestingWire* StartWire = Prev;
        while (StartWire->GetInputWire())
        {
            StartWire = StartWire->GetInputWire();
            StartWire->AssignedSegment = nullptr; // wipe old segment data
        }
        CreateNewSegment(StartWire);
    }

    if (Next)
    {
        Next->AssignedSegment = nullptr;
        CreateNewSegment(Next);
    }
	*/
}

AWireSegment* UWireSubsystem::CreateNewSegment(ATestingWire* StartWire)
{
	if (!StartWire) return nullptr;

	FActorSpawnParameters SpawnParams;
	AWireSegment* NewSegment = GetWorld()->SpawnActor<AWireSegment>(AWireSegment::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (NewSegment)
	{
		NewSegment->InitializeSegment(StartWire);
		ActiveSegments.Add(NewSegment);
		
		// Assign the segment pointer to all wires in the chain
		ATestingWire* CurrentWire = StartWire;
		while (CurrentWire)
		{
			CurrentWire->AssignedSegment = NewSegment;
			CurrentWire = CurrentWire->GetOutputWire();
		}
	}

	return NewSegment;
}
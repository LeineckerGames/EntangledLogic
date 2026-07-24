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

		// Defensive checks
		if (!FirstSegment || !SecondSegment || !SecondSegment->SplineComponent || !FirstSegment->SplineComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("AddWireToPaths: invalid segment or missing spline component"));
			return;
		}

		// Merge SecondSegment into FirstSegment
		NewWire->AssignedSegment = FirstSegment;

		// TODO Move code into a function
		TArray<FWireItemData> AllQubitsOnSecondSegment;

		// Save to an array all qubits that are before the deletion wire (copied from RemoveWireFromMiddleOfSegment)
		float DistanceFromEndOfSegment = SecondSegment->HeadGap;
		int CurrentQubitIndex = 0;

		// Do not get qubits from the segment if there are no qubits
		if (SecondSegment->ItemsOnWire.Num() > 0) {
			while (true)
			{
				AllQubitsOnSecondSegment.Add(SecondSegment->ItemsOnWire[CurrentQubitIndex]); // Collect all qubits that appear before the wire to delete
				if (CurrentQubitIndex == 0) AllQubitsOnSecondSegment[0].GapToNextItem = SecondSegment->HeadGap; // Temporarily store HeadGap into this spot to retrieve it later when making the segment.

				if (SecondSegment->ItemsOnWire.IsValidIndex(CurrentQubitIndex + 1))
				{
					DistanceFromEndOfSegment += SecondSegment->ItemsOnWire[CurrentQubitIndex + 1].GapToNextItem;
					CurrentQubitIndex++;
				}
				else
				{
					break;
				}
			}
		}

		// Update all wires in the second segment to point to the first, and add the spline points to the first
		ATestingWire* CurrentWire = NewWire;
		ATestingWire* LastIterationWire = CurrentWire;
		while (CurrentWire)
		{
			LastIterationWire = CurrentWire;
			CurrentWire->AssignedSegment = FirstSegment;

			// Take the wire's spline points and add them to the end of the wire segment
			for (int i = 0; i < CurrentWire->WireSpline->GetNumberOfSplinePoints(); i++)
			{
				FirstSegment->SplineComponent->AddSplinePoint(
					CurrentWire->WireSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World),
					ESplineCoordinateSpace::World,
					false
				);

				float PointIndex = FirstSegment->SplineComponent->GetNumberOfSplinePoints() - 1;

				FirstSegment->SplineComponent->SetRotationAtSplinePoint(
					PointIndex,
					CurrentWire->WireSpline->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World),
					ESplineCoordinateSpace::World
				);

				/*
				SplineComponent->SetScaleAtSplinePoint(
					PointIndex,
					CurrentWire->WireSpline->GetScaleAtSplinePoint(i),
					ESplineCoordinateSpace::World
				);
				*/

				// Making it linear so it flows cleanly block-to-block, adjust as needed
				FirstSegment->SplineComponent->SetSplinePointType(PointIndex, ESplinePointType::Linear);
			}

			CurrentWire = CurrentWire->GetOutputWire();
		}

		FirstSegment->SplineComponent->UpdateSpline();
		FirstSegment->EndWire = LastIterationWire;

		FirstSegment->ActiveGapIndex = SecondSegment->ActiveGapIndex;
		FirstSegment->bIsFrontBlocked = SecondSegment->bIsFrontBlocked;

		float OldHeadGap = 0.0f;
		// HeadGap: if caller provided data that used the RemoveWireFromMiddleOfSegment convention,
		// the caller has stored the HeadGap in AllQubitsOnSecondSegment[0].GapToNextItem.
		if (AllQubitsOnSecondSegment.Num() > 0)
		{
			UE_LOG(LogTemp, Display, TEXT("if ItemData.Num() > 0"));
			OldHeadGap = FirstSegment->HeadGap;
			FirstSegment->HeadGap = AllQubitsOnSecondSegment[0].GapToNextItem;
			AllQubitsOnSecondSegment[0].GapToNextItem = 0;

			// Copy items into the new segment. We create new mesh components rather than trying to reparent existing components
		// because component ownership (Outer) is tied to the actor; creating fresh components keeps ownership correct.
			for (int32 i = AllQubitsOnSecondSegment.Num() - 1; i >= 0; --i)
			{
				FWireItemData NewItem = AllQubitsOnSecondSegment[i];

				// Create a fresh mesh component for the new segment (do not attempt to move the original component between actors)
				UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(FirstSegment);
				if (NewMeshComp)
				{
					// Prefer copying the source mesh if available, otherwise fall back to the segment's testing mesh (like AddItemToWire does)
					if (AllQubitsOnSecondSegment[i].ItemMesh && AllQubitsOnSecondSegment[i].ItemMesh->GetStaticMesh())
					{
						NewMeshComp->SetStaticMesh(AllQubitsOnSecondSegment[i].ItemMesh->GetStaticMesh());
					}

					NewMeshComp->SetupAttachment(FirstSegment->SplineComponent);
					NewMeshComp->RegisterComponent();
				}

				NewItem.ItemMesh = NewMeshComp;

				// Ensure front-most item uses the convention GapToNextItem == 0
				if (i == 0)
				{
					NewItem.GapToNextItem = 0.0f;
				}

				FirstSegment->ItemsOnWire.Insert(NewItem, 0);
			}
		}
		else 
		{
			FirstSegment->HeadGap += SecondSegment->SplineComponent->GetSplineLength() + (FirstSegment->SingleWireLength);
		}

		

		if (AllQubitsOnSecondSegment.Num() > 0) 
		{
			// Guard the index access to avoid out-of-bounds when no items were copied
			int TargetIndex = CurrentQubitIndex + 1;
			if (FirstSegment->ItemsOnWire.IsValidIndex(TargetIndex))
			{
				FirstSegment->ItemsOnWire[TargetIndex].GapToNextItem = OldHeadGap + (FirstSegment->SingleWireLength) + (SecondSegment->SplineComponent->GetSplineLength() - DistanceFromEndOfSegment);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AddWireToPaths: target item index %d invalid when setting GapToNextItem"), TargetIndex);
			}
		}

		/*
		// Position item meshes along the spline immediately if the spline has points
		if (FirstSegment->ItemsOnWire.Num() > 0 && FirstSegment->SplineComponent->GetNumberOfSplinePoints() > 0)
		{
			float dist = FirstSegment->HeadGap;

			for (int32 i = 0; i < FirstSegment->ItemsOnWire.Num(); ++i)
			{
				if (i > 0)
				{
					dist += FirstSegment->ItemsOnWire[i].GapToNextItem;
				}

				if (FirstSegment->ItemsOnWire[i].ItemMesh)
				{
					FVector Loc = FirstSegment->SplineComponent->GetLocationAtDistanceAlongSpline(dist, ESplineCoordinateSpace::World);
					FRotator Rot = FirstSegment->SplineComponent->GetRotationAtDistanceAlongSpline(dist, ESplineCoordinateSpace::World);
					FirstSegment->ItemsOnWire[i].ItemMesh->SetWorldLocationAndRotation(Loc, Rot);
				}
			}
		}
		*/

		// Remove the old second segment
		ActiveSegments.Remove(SecondSegment);
		SecondSegment->Destroy();

		// Is there still residual left from the qubits from the second segment
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
		// Clean up spline points.
		TArray<FWireItemData> temp = RemovedWire->AssignedSegment->RemoveWireFromMiddleOfSegment(RemovedWire); 
		
		// Clean up wire pointers.
		RemovedWire->AssignedSegment = nullptr;
		ATestingWire* CurrentWire = Next;
		while (CurrentWire)
		{
			CurrentWire->AssignedSegment = nullptr;
			CurrentWire = CurrentWire->GetOutputWire();
		}

		// Make a second segment.
		CreateNewSegmentFromItemData(Next, temp);

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

AWireSegment* UWireSubsystem::CreateNewSegmentFromItemData(ATestingWire* StartWire, const TArray<FWireItemData>& ItemData)
{
	if (!StartWire || !GetWorld()) return nullptr;

	FActorSpawnParameters SpawnParams;
	AWireSegment* NewSegment = GetWorld()->SpawnActor<AWireSegment>(AWireSegment::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (!NewSegment) return nullptr;

	if (!NewSegment->SplineComponent)
	{
		NewSegment->Destroy();
		return nullptr;
	}

	// Initialize spline from the provided start wire (mirror behavior of CreateNewSegment)
	NewSegment->InitializeSegment(StartWire);
	ActiveSegments.Add(NewSegment);

	// Assign the segment pointer to all wires in the chain
	ATestingWire* CurrentWire = StartWire;
	while (CurrentWire)
	{
		CurrentWire->AssignedSegment = NewSegment;
		CurrentWire = CurrentWire->GetOutputWire();
	}

	// Prepare segment item state
	NewSegment->ItemsOnWire.Empty();
	NewSegment->ActiveGapIndex = 1;
	NewSegment->bIsFrontBlocked = false;

	// HeadGap: if caller provided data that used the RemoveWireFromMiddleOfSegment convention,
	// the caller has stored the HeadGap in ItemData[0].GapToNextItem.
	if (ItemData.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("if ItemData.Num() > 0 && ItemData[0].GapToNextItem > 0.0f"))
		NewSegment->HeadGap = ItemData[0].GapToNextItem;
	}

	// Copy items into the new segment. We create new mesh components rather than trying to reparent existing components
	// because component ownership (Outer) is tied to the actor; creating fresh components keeps ownership correct.
	for (int32 i = 0; i < ItemData.Num(); ++i)
	{
		FWireItemData NewItem = ItemData[i];

		// Create a fresh mesh component for the new segment (do not attempt to move the original component between actors)
		UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(NewSegment);
		if (NewMeshComp)
		{
			// Prefer copying the source mesh if available, otherwise fall back to the segment's testing mesh (like AddItemToWire does)
			if (ItemData[i].ItemMesh && ItemData[i].ItemMesh->GetStaticMesh())
			{
				NewMeshComp->SetStaticMesh(ItemData[i].ItemMesh->GetStaticMesh());
			}
			else if (NewSegment->TestingItemMesh)
			{
				UE_LOG(LogTemp, Display, TEXT("Using TestingItemMesh"));
				NewMeshComp->SetStaticMesh(NewSegment->TestingItemMesh);
			}

			NewMeshComp->SetupAttachment(NewSegment->SplineComponent);
			NewMeshComp->RegisterComponent();
		}

		NewItem.ItemMesh = NewMeshComp;

		// Ensure front-most item uses the convention GapToNextItem == 0
		if (i == 0)
		{
			NewItem.GapToNextItem = 0.0f;
		}

		NewSegment->ItemsOnWire.Add(NewItem);
	}

	// Position item meshes along the spline immediately if the spline has points
	if (NewSegment->ItemsOnWire.Num() > 0 && NewSegment->SplineComponent->GetNumberOfSplinePoints() > 0)
	{
		float CurrentDistanceAlongSpline = NewSegment->HeadGap;

		for (int32 i = 0; i < NewSegment->ItemsOnWire.Num(); ++i)
		{
			if (i > 0)
			{
				CurrentDistanceAlongSpline += NewSegment->ItemsOnWire[i].GapToNextItem;
			}

			if (NewSegment->ItemsOnWire[i].ItemMesh)
			{
				FVector Loc = NewSegment->SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
				FRotator Rot = NewSegment->SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
				NewSegment->ItemsOnWire[i].ItemMesh->SetWorldLocationAndRotation(Loc, Rot);
			}
		}
	}

	return NewSegment;
}
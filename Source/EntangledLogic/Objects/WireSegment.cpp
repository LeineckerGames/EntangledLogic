#include "WireSegment.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"
#include "EntangledLogic/Core/DevSettings/FactorySettings.h"
#include "TestingWire.h"

AWireSegment::AWireSegment()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	RootComponent = SplineComponent;
}

void AWireSegment::BeginPlay()
{
	Super::BeginPlay();

	// Allow the actor to receive input
	EnableInput(GetWorld()->GetFirstPlayerController());


	UWorld* World = GetWorld();
	if (World)
	{
		UFactorySubsystem* FactorySubsystem = World->GetSubsystem<UFactorySubsystem>();
		if (FactorySubsystem)
		{
			FactorySubsystem->OnFactoryTick.AddUObject(this, &AWireSegment::OnFactoryTick);
		}
	}

	// Bind the Y key to the AddTestingItemToWire function
	// Bind the U key to the ToggleOutput function

	// Commenting this out bc we shouldnt be adding qubits manually to the wire anymore
	// 
	//if (InputComponent)
	//{
	//	FInputKeyBinding& Binding = InputComponent->BindKey(EKeys::Y, IE_Pressed, this, &AWireSegment::AddTestingItemToWire);
	//	
	//	// Tell Unreal NOT to consume the input so other actors can hear it tooWhaWhat d
	//	Binding.bConsumeInput = false;
	//}
}

void AWireSegment::OnFactoryTick()
{
	if (IsQubitAtEndOfSpline())
	{
		OutputQubits();
	}
}

void AWireSegment::OutputQubits()
{
	UFactoryOutputComponent* LastWireOutComp = EndWire->GetOutputComponents()[0];
	if (LastWireOutComp)
	{
		AActor* CurrentActor = LastWireOutComp->OutputSlot;
		if (CurrentActor)
		{
			//UE_LOG(LogTemp, Display, TEXT("Found Actor to send Qubit: %s"), *CurrentActor->GetActorNameOrLabel());
			IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(LastWireOutComp->OutputSlot);
			if (IOInterface)
			{
				// Need a way to get the slot index from other actor and then use it here
				UFactoryInputComponent* ConnectedInputComponent = IOInterface->GetConnectedInputComponent(LastWireOutComp);
				if (ConnectedInputComponent)
				{
					int32 InputSlotIndex = ConnectedInputComponent->SlotIndex;
					//UE_LOG(LogTemp, Display, TEXT("The input comp of %s has a slot index of %d"), *ConnectedInputComponent->GetOwner()->GetActorNameOrLabel(), InputSlotIndex);
					if (IOInterface->IsQubitSlotEmpty(InputSlotIndex))
					{
						AQubit* PoppedQubit = RemoveFrontItem();
						if (PoppedQubit)
						{
							IOInterface->TransferQubit(PoppedQubit, InputSlotIndex);
						}
					}
				}
			}
		}
	
	}
}

bool AWireSegment::IsQubitAtEndOfSpline() const
{
	// try using bIsFrontBlocked instead
	if (HeadGap <= 0.0f && !ItemsOnWire.IsEmpty())
	{
		return true;
	}
	return false;
}

void AWireSegment::InitializeSegment(ATestingWire* NewStartWire)
{
	if (!NewStartWire) return;

	StartWire = NewStartWire;
	SplineComponent->ClearSplinePoints();

	ATestingWire* CurrentWire = NewStartWire;
	int32 PointIndex = 0;

	// Traverse the linked list of wires
	while (CurrentWire != nullptr)
	{
		// Add spline points at each wire's world location
		SplineComponent->AddSplinePoint(CurrentWire->GetActorLocation() + SplineOffset, ESplineCoordinateSpace::World, false);

		// Making it linear so it flows cleanly block-to-block, adjust as needed
		SplineComponent->SetSplinePointType(PointIndex, ESplinePointType::Linear);

		EndWire = CurrentWire;
		CurrentWire = CurrentWire->GetOutputWire();
		PointIndex++;
	}

	SplineComponent->UpdateSpline();
}

void AWireSegment::AddWireToEndOfSegment(ATestingWire* WireToAdd)
{
	if (!WireToAdd) return;

	SplineComponent->AddSplinePoint(WireToAdd->GetActorLocation() + SplineOffset, ESplineCoordinateSpace::World, false);

	EndWire = WireToAdd;

	SplineComponent->UpdateSpline();

	HeadGap += SingleWireLength;
	bIsFrontBlocked = false;
}

void AWireSegment::AddWireToStartOfSegment(ATestingWire* WireToAdd) {

	if (!WireToAdd) return;

	SplineComponent->AddSplinePointAtIndex(WireToAdd->GetActorLocation() + SplineOffset, 0, ESplineCoordinateSpace::World);

	StartWire = WireToAdd;

	SplineComponent->UpdateSpline();
}

void AWireSegment::RemoveWireFromEndOfSegment(ATestingWire* WireToRemove) {

	if (!WireToRemove) return;

	// Remove qubits that are present on top of this removed ending tile
	float DistanceFromEndOfSegment = 0.0f;

	DistanceFromEndOfSegment += HeadGap;
	while (DistanceFromEndOfSegment <= SingleWireLength)
	{
		// If there is at least one more item behind the front, use its gap value.
		if (ItemsOnWire.Num() > 1)
		{
			DistanceFromEndOfSegment += ItemsOnWire[1].GapToNextItem; // Use index 1 because that is the next gap.
			// The qubit is still inside this removed ending tile, so delete it.
			// Also adjust the array of items so that the gaps between items are still correct (handled by RemoveFrontItem).
			AQubit* RemovedQubit = RemoveFrontItem();
			(void)RemovedQubit;
			// After this point the indices have shifted and index 1 no longer points to the same gap as before.
		}
		else
		{
			// Only a single item remains. Remove it and exit the loop.
			AQubit* RemovedQubit = RemoveFrontItem();
			(void)RemovedQubit;
			break;
		}
	}
	HeadGap -= SingleWireLength;

	// Remove this ending wire tile from the spline
	SplineComponent->RemoveSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 1, false);

	// Set the previous wire as the new ending wire
	EndWire = WireToRemove->GetInputWire();

	SplineComponent->UpdateSpline();
}

void AWireSegment::RemoveWireFromStartOfSegment(ATestingWire* WireToRemove) 
{
	if (!WireToRemove) return;

	// Remove qubits that are present on top of this removed starting tile
	float DistanceFromEndOfSegment = 0.0f;
	ATestingWire* CurrentWire = EndWire;


	DistanceFromEndOfSegment += HeadGap;

	int CurrentQubitIndex = 1;
	int CurrentWireIndex = 0;
	CurrentWireIndex = (int)(DistanceFromEndOfSegment / SingleWireLength);
	int OldWireIndex = CurrentWireIndex;

	while (CurrentWire)
	{
		if (CurrentWire == WireToRemove) 
		{
			DistanceFromEndOfSegment += ItemsOnWire[CurrentQubitIndex + 1].GapToNextItem;
			RemoveQubitAtIndex(CurrentQubitIndex);
			// CurrentQubitIndex++;
		}
		else {
			DistanceFromEndOfSegment += ItemsOnWire[CurrentQubitIndex].GapToNextItem;
			CurrentQubitIndex++;
		}	

		CurrentWireIndex = (int)(DistanceFromEndOfSegment / SingleWireLength);
		if (CurrentWireIndex != OldWireIndex) {
			int n = CurrentWireIndex - OldWireIndex;
			for (int i = 0; i < n; i++) 
			{
				CurrentWire = CurrentWire->GetInputWire();
			}
		}
		OldWireIndex = CurrentWireIndex;
	}

	/*
	// If there is at least one more item behind the front, use its gap value.
		if (ItemsOnWire.Num() > 1)
		{
			DistanceFromEndOfSegment += ItemsOnWire[1].GapToNextItem; // Use index 1 because that is the next gap.
			
		}
		else
		{
			// Only a single item remains. Remove it and exit the loop.
			AQubit* RemovedQubit = RemoveFrontItem();
			(void)RemovedQubit;
			break;
		}
	*/

	// Remove this starting wire tile from the spline
	SplineComponent->RemoveSplinePoint(0, false);

	// Set the previous wire as the new ending wire
	StartWire = WireToRemove->GetOutputWire();

	SplineComponent->UpdateSpline();
}

void AWireSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If there are no items, skip doing math
	if (ItemsOnWire.IsEmpty()) return;

	float MoveAmount = MovementSpeed * DeltaTime;

	// DATA LOGIC
	if (!bIsFrontBlocked)
	{
		// Traffic is moving smoothly
		HeadGap -= MoveAmount;

		// Check if the front item hit the end of the line
		// Use a while loop in case multiple items exit in a single frame
		while (HeadGap <= 0.0f && !ItemsOnWire.IsEmpty())
		{
			// Attempt to leave the segment
			if (LeaveWireSegment())
			{
				// Successfully left! RemoveFrontItem() is assumed to have been called.
				// The while loop will evaluate the new HeadGap of the new front item next.
			}
			else
			{
				// LeaveWireSegment failed or returned false. Stop moving and block the line.
				HeadGap = 0.0f;
				bIsFrontBlocked = true;
				break;
			}
		}
	}
	else
	{
		// Traffic jam! Start compressing the items together.
		if (ItemsOnWire.IsValidIndex(ActiveGapIndex))
		{
			ItemsOnWire[ActiveGapIndex].GapToNextItem -= MoveAmount;

			if (ItemsOnWire[ActiveGapIndex].GapToNextItem <= ItemSize)
			{
				ItemsOnWire[ActiveGapIndex].GapToNextItem = ItemSize;
				ActiveGapIndex++;
			}
		}
	}

	// VISUAL LOGIC
	// Calculate the exact distances on the fly to place our meshes
	float CurrentDistanceAlongSpline = SplineComponent->GetSplineLength() - HeadGap;

	for (int32 i = 0; i < ItemsOnWire.Num(); i++)
	{
		if (i > 0)
		{
			CurrentDistanceAlongSpline -= ItemsOnWire[i].GapToNextItem;
		}

		FVector Loc = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
		FRotator Rot = SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);

		ItemsOnWire[i].ItemMesh->SetWorldLocationAndRotation(Loc, Rot);
	}
}

bool AWireSegment::LeaveWireSegment()
{
	// TODO: Attempt to push the front-most item out of the wire segment.
	// If successful, make sure to call RemoveFrontItem() and return true.

	// Returning false emulates the segment being physically blocked and full.
	return false;
}

bool AWireSegment::AddItemToWire(AQubit* QubitData)
{
	if (Capacity > 0 && ItemsOnWire.Num() >= Capacity)
	{
		return false;
	}

	FWireItemData NewItem;

	NewItem.ItemMesh = NewObject<UStaticMeshComponent>(this);
	const UFactorySettings* Settings = GetDefault<UFactorySettings>();
	if (Settings && !Settings->QubitMesh.IsNull())
	{
		UStaticMesh* LoadedMesh = Settings->QubitMesh.LoadSynchronous();
		NewItem.ItemMesh->SetStaticMesh(LoadedMesh);
	}

	NewItem.ItemMesh->SetupAttachment(SplineComponent);
	NewItem.ItemMesh->RegisterComponent();
	NewItem.QubitData = QubitData;

	if (ItemsOnWire.IsEmpty())
	{
		HeadGap = SplineComponent->GetSplineLength();
		NewItem.GapToNextItem = 0.0f;
		ActiveGapIndex = 1;
	}
	else
	{
		float LastItemPos = SplineComponent->GetSplineLength() - HeadGap;
		for (int32 i = 1; i < ItemsOnWire.Num(); i++)
		{
			LastItemPos -= ItemsOnWire[i].GapToNextItem;
		}

		NewItem.GapToNextItem = LastItemPos;
	}

	ItemsOnWire.Add(NewItem);
	return true;
}


// Returns a nullptr if fails
AQubit* AWireSegment::RemoveFrontItem()
{
	AQubit* QubitToPop = nullptr;

	if (ItemsOnWire.IsEmpty()) return QubitToPop;

	if (ItemsOnWire[0].ItemMesh)
	{
		QubitToPop = ItemsOnWire[0].QubitData;
		ItemsOnWire[0].ItemMesh->DestroyComponent();
	}

	if (ItemsOnWire.Num() > 1)
	{
		HeadGap += ItemsOnWire[1].GapToNextItem;
		ItemsOnWire.RemoveAt(0);
		ItemsOnWire[0].GapToNextItem = 0.0f;
		ActiveGapIndex = FMath::Max(1, ActiveGapIndex - 1);
	}
	else
	{
		ItemsOnWire.RemoveAt(0);
		ActiveGapIndex = 1;
	}

	bIsFrontBlocked = false;
	return QubitToPop;
}

bool AWireSegment::IsEmpty()
{
	return ItemsOnWire.IsEmpty();
}

bool AWireSegment::IsFull()
{
	return Capacity > 0 && ItemsOnWire.Num() >= Capacity;
}

void AWireSegment::AddTestingItemToWire(AQubit* QubitData)
{
	AddItemToWire(QubitData);
}

AQubit* AWireSegment::RemoveQubitAtIndex(int32 Index)
{
	// Invalid index
	if (!ItemsOnWire.IsValidIndex(Index))
	{
		return nullptr;
	}

	// Capture return value
	AQubit* RemovedQubit = ItemsOnWire[Index].QubitData;

	// Destroy mesh if present
	if (ItemsOnWire[Index].ItemMesh)
	{
		ItemsOnWire[Index].ItemMesh->DestroyComponent();
	}

	// If removing the front item, replicate the adjustments made in RemoveFrontItem()
	if (Index == 0)
	{
		if (ItemsOnWire.Num() > 1)
		{
			// Add the gap of the (soon-to-be) new front item to HeadGap
			HeadGap += ItemsOnWire[1].GapToNextItem;

			// Remove the front item
			ItemsOnWire.RemoveAt(0);

			// Ensure the new front has zero gap-to-next (by convention)
			if (ItemsOnWire.Num() > 0)
			{
				ItemsOnWire[0].GapToNextItem = 0.0f;
			}

			ActiveGapIndex = FMath::Max(1, ActiveGapIndex - 1);
		}
		else
		{
			// Last item removed
			ItemsOnWire.RemoveAt(0);
			ActiveGapIndex = 1;
		}

		// Clearing front-blocked state as front item no longer blocks
		bIsFrontBlocked = false;
	}
	else
	{
		// Removing from middle or end:
		const int32 NumBefore = ItemsOnWire.Num();

		// If there is an item behind the removed one, it must inherit the removed gap so that distances stay consistent.
		// gap_{k+1} (old) becomes gap_{k+1} + gap_k (old), where gap_k is ItemsOnWire[Index].GapToNextItem.
		if (Index < NumBefore - 1)
		{
			ItemsOnWire[Index + 1].GapToNextItem += ItemsOnWire[Index].GapToNextItem;
		}

		// Remove the item
		ItemsOnWire.RemoveAt(Index);

		// If the removal was before the ActiveGapIndex, shift ActiveGapIndex down so it continues to point
		// at the same logical gap as before (indexes shifted left).
		if (Index < ActiveGapIndex)
		{
			ActiveGapIndex = FMath::Max(1, ActiveGapIndex - 1);
		}
	}

	// Clamp ActiveGapIndex to a safe, valid range
	if (ItemsOnWire.Num() <= 1)
	{
		ActiveGapIndex = 1;
	}
	else
	{
		ActiveGapIndex = FMath::Clamp(ActiveGapIndex, 1, ItemsOnWire.Num() - 1);
	}

	return RemovedQubit;
}


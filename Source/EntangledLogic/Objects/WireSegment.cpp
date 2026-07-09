#include "WireSegment.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"
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
	UFactoryOutputComponent* LastWireOutComp = LastWire->GetOutputComponents()[0];
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

void AWireSegment::InitializeSegment(ATestingWire* StartWire)
{
	if (!StartWire) return;

	FirstWire = StartWire;
	SplineComponent->ClearSplinePoints();

	ATestingWire* CurrentWire = StartWire;
	int32 PointIndex = 0;

	// Traverse the linked list of wires
	while (CurrentWire != nullptr)
	{
		// Add spline points at each wire's world location
		SplineComponent->AddSplinePoint(CurrentWire->GetActorLocation(), ESplineCoordinateSpace::World, false);

		// Making it linear so it flows cleanly block-to-block, adjust as needed
		SplineComponent->SetSplinePointType(PointIndex, ESplinePointType::Linear);

		LastWire = CurrentWire;
		CurrentWire = CurrentWire->GetOutputWire();
		PointIndex++;
	}

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

bool AWireSegment::AddItemToWire(UStaticMesh* MeshToUse, AQubit* QubitData)
{
	if (Capacity > 0 && ItemsOnWire.Num() >= Capacity)
	{
		return false;
	}

	FWireItemData NewItem;

	NewItem.ItemMesh = NewObject<UStaticMeshComponent>(this);
	NewItem.ItemMesh->SetStaticMesh(MeshToUse);
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
	AddItemToWire(TestingItemMesh, QubitData);
}

////Get output factory and send qubits

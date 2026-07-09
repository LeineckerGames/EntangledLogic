#include "WireSegment.h"
#include "TestingWire.h"
#include <EntangledLogic/Core/Subsystems/FactorySubsystem.h>

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

	// Bind the Y key to the AddTestingItemToWire function
	// Bind the U key to the ToggleOutput function
	/*
	if (InputComponent)
	{
		FInputKeyBinding& Binding = InputComponent->BindKey(EKeys::Y, IE_Pressed, this, &AWireSegment::AddQubitToWire);
		
		// Tell Unreal NOT to consume the input so other actors can hear it tooWhaWhat d
		Binding.bConsumeInput = false;
	}
	*/

	UWorld* World = GetWorld();
	if (World)
	{
		UFactorySubsystem* FactorySubsystem = World->GetSubsystem<UFactorySubsystem>();
		if (FactorySubsystem)
		{
			FactorySubsystem->OnFactoryTick.AddUObject(this, &AWireSegment::OnFactoryTick);
		}
	}
}

void AWireSegment::OnFactoryTick()
{
	if (bIsFrontBlocked) 
	{
		AQubit* QubitToRemove = RemoveFrontItem();
		OutputQubit(QubitToRemove);
	}
}

void AWireSegment::OutputQubit(AQubit* QubitToSend)
{
	TArray<UFactoryOutputComponent*> OutputComponents = LastWire->GetOutputComponents();

	//Get output factory and send qubits
	int32 SlotNumber = 0;
	for (UFactoryOutputComponent* CurrentOutputComponent : OutputComponents)
	{
		if (CurrentOutputComponent->OutputSlot)
		{
			AActor* CurrentActor = CurrentOutputComponent->OutputSlot;
			if (CurrentActor)
			{
				//UE_LOG(LogTemp, Display, TEXT("Found Actor to send Qubit: %s"), *CurrentActor->GetActorNameOrLabel());
				IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(CurrentOutputComponent->OutputSlot);
				if (IOInterface)
				{
					// Need a way to get the slot index from other actor and then use it here
					UFactoryInputComponent* ConnectedInputComponent = IOInterface->GetConnectedInputComponent(CurrentOutputComponent);
					if (ConnectedInputComponent)
					{
						int32 InputSlotIndex = ConnectedInputComponent->SlotIndex;
						//UE_LOG(LogTemp, Display, TEXT("The input comp of %s has a slot index of %d"), *ConnectedInputComponent->GetOwner()->GetActorNameOrLabel(), InputSlotIndex);
						if (IOInterface->IsQubitSlotEmpty(InputSlotIndex))
						{
							// Send qubit to next factory
							
							IOInterface->TransferQubit(QubitToSend, InputSlotIndex);
							
						}
					}
				}
			}

		}
		SlotNumber++;
	}
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
			HeadGap = 0.0f;
			bIsFrontBlocked = true;
			break;
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


bool AWireSegment::AddQubitToWire(AQubit* Qubit)
{
	if (Capacity > 0 && ItemsOnWire.Num() >= Capacity)
	{
		return false;
	}

	FWireItemData NewItem;

	NewItem.ItemMesh = NewObject<UStaticMeshComponent>(this);
	NewItem.ItemMesh->SetStaticMesh(QubitMesh);
	NewItem.ItemMesh->SetupAttachment(SplineComponent);
	NewItem.ItemMesh->RegisterComponent();

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
	QubitDatasOnWire.Add(Qubit);
	return true;
}

AQubit* AWireSegment::RemoveFrontItem()
{
	if (ItemsOnWire.IsEmpty()) return nullptr;

	AQubit* QubitToRemove = QubitDatasOnWire[0];

	if (ItemsOnWire[0].ItemMesh)
	{
		ItemsOnWire[0].ItemMesh->DestroyComponent();
	}

	if (ItemsOnWire.Num() > 1)
	{
		HeadGap += ItemsOnWire[1].GapToNextItem;
		ItemsOnWire.RemoveAt(0);
		QubitDatasOnWire.RemoveAt(0);
		ItemsOnWire[0].GapToNextItem = 0.0f;
		ActiveGapIndex = FMath::Max(1, ActiveGapIndex - 1);
	}
	else
	{
		ItemsOnWire.RemoveAt(0);
		QubitDatasOnWire.RemoveAt(0);
		ActiveGapIndex = 1;
	}

	bIsFrontBlocked = false;
	return QubitToRemove;
}

bool AWireSegment::IsEmpty()
{
	return ItemsOnWire.IsEmpty();
}

bool AWireSegment::IsFull()
{
	return Capacity > 0 && ItemsOnWire.Num() >= Capacity;
}

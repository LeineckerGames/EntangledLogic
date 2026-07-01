#include "WireSegment.h"
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

	// Bind the Y key to the AddTestingItemToWire function
	// Bind the U key to the ToggleOutput function
	if (InputComponent)
	{
		FInputKeyBinding& Binding = InputComponent->BindKey(EKeys::Y, IE_Pressed, this, &AWireSegment::AddTestingItemToWire);
		
		// Tell Unreal NOT to consume the input so other actors can hear it tooWhaWhat d
		Binding.bConsumeInput = false;
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
		CurrentWire = CurrentWire->NextWire;
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

bool AWireSegment::AddItemToWire(UStaticMesh* MeshToUse)
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

void AWireSegment::RemoveFrontItem()
{
	if (ItemsOnWire.IsEmpty()) return;

	if (ItemsOnWire[0].ItemMesh)
	{
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
}

bool AWireSegment::IsEmpty()
{
	return ItemsOnWire.IsEmpty();
}

bool AWireSegment::IsFull()
{
	return Capacity > 0 && ItemsOnWire.Num() >= Capacity;
}

void AWireSegment::AddTestingItemToWire()
{
	AddItemToWire(TestingItemMesh);
}
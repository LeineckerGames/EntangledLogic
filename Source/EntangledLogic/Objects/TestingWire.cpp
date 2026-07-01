#include "TestingWire.h"

ATestingWire::ATestingWire()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	SplineComponent->SetupAttachment(WireMesh);
}

void ATestingWire::BeginPlay()
{
	Super::BeginPlay();

	// Allow the actor to receive input
	EnableInput(GetWorld()->GetFirstPlayerController());

	// Bind the Y key to the AddTestingItemToWire function
	// Bind the U key to the ToggleOutput function
	if (InputComponent)
	{
		if (CanManuallyAddItems) {
			InputComponent->BindKey(EKeys::Y, IE_Pressed, this, &ATestingWire::AddTestingItemToWire);
			InputComponent->BindKey(EKeys::U, IE_Pressed, this, &ATestingWire::ToggleOutput);
		}
		if (CanInitiateItemMovement) {
			InputComponent->BindKey(EKeys::I, IE_Pressed, this, &ATestingWire::TryMoveItemForward);
		}
	}
}

void ATestingWire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If there are no items, skip doing math
	if (ItemsOnWire.IsEmpty()) return;

	float MoveAmount = MovementSpeed * DeltaTime;

	// DATA LOGIC
	if (!bIsFrontBlocked)
	{
		// Traffic is moving smoothly. We ONLY update the front-most item's distance to the end.
		// All items behind it just keep their set distance.
		HeadGap -= MoveAmount;

		// Check if the front item hit the end of the line
		// We use a while loop here just in case multiple items exit in a single frame
		while (HeadGap <= 0.0f && !ItemsOnWire.IsEmpty())
		{

			// Belt is closed. Stop moving and block the line.
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
			// Shrink the gap of the first uncompressed item behind the jam
			ItemsOnWire[ActiveGapIndex].GapToNextItem -= MoveAmount;

			// If it shrinks to the item's physical size, it has collided with the item in front of it entirely.
			// It will stay crunched together forever, so we move backwards to compress the next gap.
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
		// Subtract the gap to find the exact position of this specific item
		if (i > 0)
		{
			CurrentDistanceAlongSpline -= ItemsOnWire[i].GapToNextItem;
		}

		// Apply the transforms visually so the player can see it
		FVector Loc = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
		FRotator Rot = SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::World);
		
		ItemsOnWire[i].ItemMesh->SetWorldLocationAndRotation(Loc, Rot);
	}
}

bool ATestingWire::AddItemToWire(UStaticMesh* MeshToUse)
{
	// Check if adding an item would exceed capacity
	if (Capacity > 0 && ItemsOnWire.Num() >= Capacity)
	{
		return false;
	}

	FWireItemData NewItem;
	
	// Create and attach a new visual component for the item
	NewItem.ItemMesh = NewObject<UStaticMeshComponent>(this);
	NewItem.ItemMesh->SetStaticMesh(MeshToUse);
	NewItem.ItemMesh->SetupAttachment(SplineComponent);
	NewItem.ItemMesh->RegisterComponent();

	if (ItemsOnWire.IsEmpty())
	{
		// It's the first item! Place it at the very start of the spline 
		HeadGap = SplineComponent->GetSplineLength(); 
		NewItem.GapToNextItem = 0.0f; 
        ActiveGapIndex = 1;
	}
	else
	{
		// Calculate how far up the spline the very last item in the line currently is
		float LastItemPos = SplineComponent->GetSplineLength() - HeadGap;
		for (int32 i = 1; i < ItemsOnWire.Num(); i++)
		{
			LastItemPos -= ItemsOnWire[i].GapToNextItem;
		}
		
		// Set the gap between the start of the spline and the last item
		NewItem.GapToNextItem = LastItemPos; 
	}

	ItemsOnWire.Add(NewItem);
	return true;
}

void ATestingWire::AddTestingItemToWire() 
{
	AddItemToWire(TestingItemMesh);
}

void ATestingWire::RemoveFrontItem()
{
	if (ItemsOnWire.IsEmpty()) return;

	// Destroy the visual mesh of the front item
	if (ItemsOnWire[0].ItemMesh)
	{
		ItemsOnWire[0].ItemMesh->DestroyComponent();
	}

	if (ItemsOnWire.Num() > 1)
	{
		// The new HeadGap is our current HeadGap PLUS the distance to the next in-line item
		HeadGap += ItemsOnWire[1].GapToNextItem;

		// Remove the old front item so Index 1 becomes the new Index 0
		ItemsOnWire.RemoveAt(0);

		// The new front item doesn't have an item in front of it anymore, zero it out for consistency
		ItemsOnWire[0].GapToNextItem = 0.0f;

		// Shift our ActiveGapIndex down one, but keep it a minimum of 1
		ActiveGapIndex = FMath::Max(1, ActiveGapIndex - 1);
	}
	else
	{
		// It was the only item on the belt
		ItemsOnWire.RemoveAt(0);
		ActiveGapIndex = 1;
	}

	// Always unblock the front of the belt when an item leaves it
	bIsFrontBlocked = false;
}

void ATestingWire::ToggleOutput()
{
	bCanOutput = !bCanOutput; // Flip the switch!

	// If we just unblocked the line and items were jammed at the front, we need to unblock it so it flow again
	if (bCanOutput && bIsFrontBlocked)
	{
		bIsFrontBlocked = false;
	}
}

bool ATestingWire::IsEmpty()
{
	return ItemsOnWire.IsEmpty();
}

bool ATestingWire::IsFull()
{
	return Capacity > 0 && ItemsOnWire.Num() >= Capacity;
}

void ATestingWire::TryMoveItemForward()
{
	UE_LOG(LogTemp, Display, TEXT("TryMoveItemForward()"));
	if (NextWire != nullptr) 
	{
		if (!this->IsEmpty())
		{
			UE_LOG(LogTemp, Display, TEXT("this is not empty"));
			if (NextWire->AddItemToWire(TestingItemMesh)) // Try to add item to the next wire.
			{
				this->RemoveFrontItem(); // Remove an item from me. The item transferred from me to the next wire.
			}
			// Else will fail, the item could not be added to the next wire because the next wire is full and has no more room.
		}
	}
	// Ask the previous wire to an item into me
	if (PreviousWire != nullptr)
	{
		PreviousWire->TryMoveItemForward();
	}
}


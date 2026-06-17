


#include "Wire.h"
#include "EntangledLogic/Core/Components/GridPlacementComponent.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"

// Sets default values
AWire::AWire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Root Component
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");

	SetRootComponent(DefaultRoot);

	// Create Mesh and attach to root
	WireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FactoryMesh"));
	WireMesh->SetupAttachment(RootComponent);

	// Create Grid Placement and attach to mesh
	GridPlacementComponent = CreateDefaultSubobject<UGridPlacementComponent>(TEXT("GridPlacementComponent"));
	GridPlacementComponent->SetupAttachment(WireMesh);


}

// Called when the game starts or when spawned
void AWire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Factory Interaction Interface
void AWire::BeginHover(EPlacementMode PlacementMode)
{
	//UE_LOG(LogTemp, Display, TEXT("Hovered On %s"), *GetActorNameOrLabel());
	switch (PlacementMode)
	{
	case EPlacementMode::Disabled:
		// Make Placing outline for clicking on factories to open UI
		GridPlacementComponent->EnableInteractionOutline();
		break;
	case EPlacementMode::Placing:

		break;
	case EPlacementMode::Editing:
		GridPlacementComponent->EnableEditOutline();
		break;
	case EPlacementMode::Deletion:
		GridPlacementComponent->EnableDeleteOutline();
		break;
	}
}

void AWire::EndHover(EPlacementMode PlacementMode)
{
	//UE_LOG(LogTemp, Display, TEXT("Exited %s"), *GetActorNameOrLabel());
	switch (PlacementMode)
	{
	case EPlacementMode::Disabled:
		// Make Placing outline for clicking on factories to open UI
		GridPlacementComponent->DisableOutline();
		break;
	case EPlacementMode::Placing:
		GridPlacementComponent->DisableOutline();
		break;
	case EPlacementMode::Editing:
		GridPlacementComponent->DisableOutline();
		break;
	case EPlacementMode::Deletion:
		GridPlacementComponent->DisableOutline();
		break;
	}
}

void AWire::Interact(EPlacementMode PlacementMode)
{
	UE_LOG(LogTemp, Display, TEXT("Clicked on %s"), *GetActorNameOrLabel());
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	switch (PlacementMode)
	{
	case EPlacementMode::Disabled:
		UE_LOG(LogTemp, Display, TEXT("Selecting Actor %s"), *GetActorNameOrLabel());
		// FactoryDisplayWindow->ToggleVisibility();

		// Open selected pop up UI
		break;
	case EPlacementMode::Placing:

		break;
	case EPlacementMode::Editing:
	{
		//UE_LOG(LogTemp, Display, TEXT("Picking up Actor %s"), *GetActorNameOrLabel());
		// Remove from collision map
		TArray<FGridCoordinate> GridLocations = GridPlacement->GridComponentToCoordinates(GridPlacementComponent);
		GridPlacement->SetPlacedPositionMap(GridLocations, GridPlacementComponent->GetFactoryShape(), false);

		GridPlacement->PickupFactory(this);
	} break;
	case EPlacementMode::Deletion:
	{
		//UE_LOG(LogTemp, Display, TEXT("Deleting Actor %s"), *GetActorNameOrLabel());
		// Update Collision and then delete
		TArray<FGridCoordinate> GridLocations = GridPlacement->GridComponentToCoordinates(GridPlacementComponent);
		GridPlacement->SetPlacedPositionMap(GridLocations, GridPlacementComponent->GetFactoryShape(), false);
		Destroy();
	} break;
	}
}





#include "FactoryBase.h"
#include "EntangledLogic/Core/Components/GridPlacementComponent.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"

// Sets default values
AFactoryBase::AFactoryBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Root Component
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");

	SetRootComponent(DefaultRoot);

	// Create Mesh and attach to root
	FactoryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FactoryMesh"));
	FactoryMesh->SetupAttachment(RootComponent);

	// Create Grid Placement and attach to mesh
	GridPlacementComponent = CreateDefaultSubobject<UGridPlacementComponent>(TEXT("GridPlacementComponent"));
	GridPlacementComponent->SetupAttachment(FactoryMesh);

}

// Called when the game starts or when spawned
void AFactoryBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFactoryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Factory Interaction Interface
void AFactoryBase::BeginHover(EPlacementMode PlacementMode)
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

void AFactoryBase::EndHover(EPlacementMode PlacementMode)
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

void AFactoryBase::Interact(EPlacementMode PlacementMode)
{
	UE_LOG(LogTemp, Display, TEXT("Clicked on %s"), *GetActorNameOrLabel());
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	switch (PlacementMode)
	{
		case EPlacementMode::Disabled:
			UE_LOG(LogTemp, Display, TEXT("Selecting Actor %s"), *GetActorNameOrLabel());
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



#include "FactoryBase.h"
#include "EntangledLogic/Core/Components/GridPlacementComponent.h"

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

}

void AFactoryBase::EndHover(EPlacementMode PlacementMode)
{

}

void AFactoryBase::Interact(EPlacementMode PlacementMode)
{

}
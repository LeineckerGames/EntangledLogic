#include "EntangledLogic/Objects/Qubits/QubitPreview.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"

// Sets default values
AQubitPreview::AQubitPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Root Component
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
	SetRootComponent(DefaultRoot);

	// Create Mesh and attach to root
	QubitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QubitMesh"));
	QubitMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AQubitPreview::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQubitPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQubitPreview::UpdateMeshData()
{
	QubitMesh->SetCustomPrimitiveDataVector3(0, State.GetBlochVector());
}




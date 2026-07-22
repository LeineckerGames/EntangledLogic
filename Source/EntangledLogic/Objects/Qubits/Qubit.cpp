#include "EntangledLogic/Objects/Qubits/Qubit.h"
#include "EntangledLogic/Core/Framework/QubitDataStructs.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "Templates/SharedPointer.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "QppPlugin.h"

// Sets default values
AQubit::AQubit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Root Component
	//DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
	//SetRootComponent(DefaultRoot);

	// Create Mesh and attach to root
	QubitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QubitMesh"));
	SetRootComponent(QubitMesh);
	//QubitMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AQubit::BeginPlay()
{
	Super::BeginPlay();
	//State = MakeShared<FQubitData>();
}

// Called every frame
void AQubit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UpdateSplineMovement)
	{
		State->MoveEntanglementSpline();
	}

}

void AQubit::AttachEntanglementSplineToQubit(AQubit* QubitToAttach)
{
	// Create a new spline component
	USplineComponent* EntanglementSpline = NewObject<USplineComponent>(this);
	EntanglementSpline->SetupAttachment(GetRootComponent());
	EntanglementSpline->RegisterComponent();
	EntanglementSpline->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	// Set spline point locations to the corresponding qubits
	EntanglementSpline->SetLocationAtSplinePoint(0, GetActorLocation(), ESplineCoordinateSpace::World, true);
	EntanglementSpline->SetLocationAtSplinePoint(1, QubitToAttach->GetActorLocation(), ESplineCoordinateSpace::World, true);

	// Make Spline Mesh and setup start / end points and attach to SplineComp
	USplineMeshComponent* EntanglementSplineMeshComponent = NewObject<USplineMeshComponent>(this);
	EntanglementSplineMeshComponent->SetMobility(EComponentMobility::Movable);
	EntanglementSplineMeshComponent->SetupAttachment(EntanglementSpline);
	EntanglementSplineMeshComponent->RegisterComponent();

	if (EntanglementSplineMesh)
	{
		EntanglementSplineMeshComponent->SetStaticMesh(EntanglementSplineMesh);
	}

	// Get start / end spline point info
	FVector StartPos, StartTangent, EndPos, EndTangent;
	EntanglementSpline->GetLocationAndTangentAtSplinePoint(0, StartPos, StartTangent, ESplineCoordinateSpace::Local);
	EntanglementSpline->GetLocationAndTangentAtSplinePoint(1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

	// Apply info to spline mesh comp
	EntanglementSplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	EntanglementSplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);

	// Add to actor and splines array
	AddInstanceComponent(EntanglementSplineMeshComponent);
	AddInstanceComponent(EntanglementSpline);

	EntanglementSplines.Add(EntanglementSpline);
}

void AQubit::UpdateEntanglementSplinePointLocation(FVector EndSplinePointLocation, int32 SplineIndex)
{
	// Set spline point locations to the corresponding qubits

	EntanglementSplines[SplineIndex]->SetLocationAtSplinePoint(0, GetActorLocation(), ESplineCoordinateSpace::World, true);
	EntanglementSplines[SplineIndex]->SetLocationAtSplinePoint(1, EndSplinePointLocation, ESplineCoordinateSpace::World, true);

	// Get start / end spline point info
	FVector StartPos, StartTangent, EndPos, EndTangent;
	EntanglementSplines[SplineIndex]->GetLocationAndTangentAtSplinePoint(0, StartPos, StartTangent, ESplineCoordinateSpace::Local);
	EntanglementSplines[SplineIndex]->GetLocationAndTangentAtSplinePoint(1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

	// Apply info to spline mesh comp
	USplineMeshComponent* EntanglementSplineMeshComponent = Cast<USplineMeshComponent>(EntanglementSplines[SplineIndex]->GetChildComponent(0));
	if (EntanglementSplineMeshComponent)
	{
		EntanglementSplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
		EntanglementSplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);
	}

}

FString AQubit::GetString()
{
	// open string stream
	std::ostringstream oss1;

	// push qpp's display to the stream
	oss1 << qpp::disp(State->StateVector) << '\n';

	// string from stream
	return FString(oss1.str().c_str());
}

void AQubit::UpdateMeshData()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UQubitDataSubsystem* QubitSubsystem = World->GetSubsystem<UQubitDataSubsystem>();
		if (QubitSubsystem)
		{
			FVector BlochVector = QubitSubsystem->GetBlochVector(*this);
			QubitMesh->SetCustomPrimitiveDataVector3(0, BlochVector);
		}
	}
}

void AQubit::NotifyActorBeginCursorOver()
{
	UpdateSplineMovement = true;
	State->SetEntanglementSplineVisibility(true);
}

void AQubit::NotifyActorEndCursorOver()
{
	UpdateSplineMovement = false;
	State->SetEntanglementSplineVisibility(false);
}
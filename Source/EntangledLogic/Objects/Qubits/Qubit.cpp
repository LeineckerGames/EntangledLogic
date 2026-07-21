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

}

void AQubit::AttachEntanglementSplineToQubit(AQubit* QubitToAttach)
{
	USplineComponent* EntanglementSpline = Cast<USplineComponent>(AddComponentByClass(USplineComponent::StaticClass(), false, GetActorTransform(), false));
	EntanglementSpline->AddSplinePoint(QubitToAttach->GetActorTransform().GetLocation(), ESplineCoordinateSpace::Local, true);

	EntanglementSpline->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	// Make Spline Mesh and setup start / end points and attach to SplineComp
	USplineMeshComponent* EntanglementSplineMeshComponent = Cast<USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), false, GetActorTransform(), false));
	EntanglementSplineMeshComponent->SetupAttachment(EntanglementSpline);

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

	// Make visible in editor at runtime
	EntanglementSplineMeshComponent->RegisterComponent();
	AddInstanceComponent(EntanglementSplineMeshComponent);

	AddInstanceComponent(EntanglementSpline);

	EntanglementSplines.Add(EntanglementSpline);
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
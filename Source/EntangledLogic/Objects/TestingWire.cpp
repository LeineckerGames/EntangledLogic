#include "TestingWire.h"

ATestingWire::ATestingWire()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	SplineComponent->SetupAttachment(GetRootComponent());

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(SplineComponent);
}

void ATestingWire::BeginPlay()
{
	Super::BeginPlay();

	// Allow the actor to receive input
	EnableInput(GetWorld()->GetFirstPlayerController());

	// Bind the Y key to the StartMovement function
	if (InputComponent)
	{
		InputComponent->BindKey(EKeys::Y, IE_Pressed, this, &ATestingWire::StartMovement);
	}

	// Initialize timeline function bindings
	if (MovementCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, TEXT("ProcessMovementTimeline"));
		MovementTimeline.AddInterpFloat(MovementCurve, ProgressFunction);

		FOnTimelineEvent OnTimelineFinishedFunction;
		OnTimelineFinishedFunction.BindUFunction(this, TEXT("OnEndMovementTimeline"));
		MovementTimeline.SetTimelineFinishedFunc(OnTimelineFinishedFunction);

		MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
	}
}

void ATestingWire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}
}

void ATestingWire::StartMovement()
{
	MovementTimeline.PlayFromStart();
}

void ATestingWire::ProcessMovementTimeline(float Value) 
{
	// Obtain number of points along the spline
	const float SplineLength = SplineComponent->GetSplineLength();

	// Capture loc and rot along the spline
	FVector CurrentSplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);

	// Move mesh to this loc and rot along the spline
	MeshComponent->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
}

void ATestingWire::OnEndMovementTimeline() 
{

}


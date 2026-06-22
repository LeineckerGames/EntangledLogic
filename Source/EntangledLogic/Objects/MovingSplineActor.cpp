// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingSplineActor.h"

// Sets default values
AMovingSplineActor::AMovingSplineActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	RootComponent = SplineComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(SplineComponent);
}

// Called when the game starts or when spawned
void AMovingSplineActor::BeginPlay()
{
	Super::BeginPlay();

	// Allow the actor to receive input
	EnableInput(GetWorld()->GetFirstPlayerController());

	// Bind the Y key to the StartMovement function
	if (InputComponent)
	{
		InputComponent->BindKey(EKeys::Y, IE_Pressed, this, &AMovingSplineActor::StartMovement);
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

// Called every frame
void AMovingSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}
}

// Starts the movement along the spline from the beginning
void AMovingSplineActor::StartMovement()
{
	MovementTimeline.PlayFromStart();
}

// Move mesh along the spline
void AMovingSplineActor::ProcessMovementTimeline(float Value) 
{
	// Obtain number of points along the spline
	const float SplineLength = SplineComponent->GetSplineLength();

	// Capture loc and rot along the spline
	FVector CurrentSplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);

	// Move mesh to this loc and rot along the spline
	MeshComponent->SetWorldLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
}

void AMovingSplineActor::OnEndMovementTimeline() 
{

}
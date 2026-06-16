#include "QuantumGnomeFactory.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "EntangledLogic/Core/Components/GridPlacementComponent.h"

AQuantumGnomeFactory::AQuantumGnomeFactory()
{
	AudioComponenet = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponenet"));
	AudioComponenet->SetupAttachment(RootComponent);

	GnomeFracturedGeometry = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GnomeFracturedGeometry"));
	GnomeFracturedGeometry->SetupAttachment(FactoryMesh);
}

void AQuantumGnomeFactory::BeginPlay()
{
	Super::BeginPlay();
	if (GnomeFracturedGeometry)
	{
		GnomeFracturedGeometry->SetVisibility(false);
		GnomeFracturedGeometry->SetSimulatePhysics(false);
	}
}

void AQuantumGnomeFactory::Explode()
{
	if (GnomeFracturedGeometry)
	{
		GnomeFracturedGeometry->SetVisibility(true);
		FactoryMesh->SetVisibility(false);
		GnomeFracturedGeometry->SetSimulatePhysics(true);
		GnomeFracturedGeometry->ApplyExternalStrain(0, FVector(0.0f, 25.0f, 0.0f), 200.0f, 5000.0f);
	}
}

void AQuantumGnomeFactory::Interact(EPlacementMode PlacementMode)
{
	Super::Interact(PlacementMode);
	if (PlacementMode == EPlacementMode::Disabled)
	{
		ClickCount++;
		if (ClickCount >= 10)
		{
			// Make gnome explode
			AudioComponenet->SetSound(YaySound);
			AudioComponenet->SetPitchMultiplier(1.0f);
			AudioComponenet->Play();
			Explode();
			UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
			TArray<FGridCoordinate> GridLocations = GridPlacement->GridComponentToCoordinates(GridPlacementComponent);
			GridPlacement->SetPlacedPositionMap(GridLocations, GridPlacementComponent->GetFactoryShape(), false);
			SetLifeSpan(5.0f);
		}
		else
		{
			AudioComponenet->SetPitchMultiplier(ClickCount / 5.0f);
			AudioComponenet->Play();
		}
	}


}

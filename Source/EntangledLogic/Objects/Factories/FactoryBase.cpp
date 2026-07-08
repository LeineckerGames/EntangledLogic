#include "FactoryBase.h"
#include "Components/WidgetComponent.h"
#include "EntangledLogic/Core/Components/GridPlacementComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"
#include "EntangledLogic/Core/Framework/SortBySlotIndex.h"
#include "EntangledLogic/UI/Factory/FactoryInfoUI.h"
#include "EntangledLogic/UI/Factory/FactoryDevUI.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/PlayerCameraManager.h"



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

	// Creates the UI above the factory
	FactoryDisplayWindow = CreateDefaultSubobject<UWidgetComponent>(TEXT("FactoryDisplayWindow"));
	FactoryDisplayWindow->SetupAttachment(RootComponent);
	FactoryDisplayWindow->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	FactoryDisplayWindow->SetWidgetSpace(EWidgetSpace::Screen);
	FactoryDisplayWindow->SetDrawAtDesiredSize(true);

	// Create Grid Placement and attach to mesh
	GridPlacementComponent = CreateDefaultSubobject<UGridPlacementComponent>(TEXT("GridPlacementComponent"));
	GridPlacementComponent->SetupAttachment(FactoryMesh);
}

// Called when the game starts or when spawned
void AFactoryBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		UFactorySubsystem* FactorySubsystem = World->GetSubsystem<UFactorySubsystem>();
		if (FactorySubsystem)
		{
			FactorySubsystem->OnFactoryTick.AddUObject(this, &AFactoryBase::OnFactoryTick);
		}
	}

	//Qubits.SetNum(NUM_QUBIT_SLOTS);
	// Get Attached Inputs and Outputs and add them to the array
	GetComponents<UFactoryInputComponent>(InputComponents);
	GetComponents<UFactoryOutputComponent>(OutputComponents);

	// Sort arrays by input / output slot
	InputComponents.Sort(SortBySlotIndex<UFactoryInputComponent>);
	OutputComponents.Sort(SortBySlotIndex<UFactoryOutputComponent>);

	// Setup Floating UI
	FactoryDisplayWindow->SetVisibility(false);
	FactoryWidget = FactoryDisplayWindow->GetUserWidgetObject();
	UFactoryInfoUI* FactoryInfoWidget = nullptr;
	UFactoryDevUI* FactoryDevWidget = nullptr;
	if (FactoryWidget)
	{
		FactoryInfoWidget = Cast<UFactoryInfoUI>(FactoryWidget);
		FactoryDevWidget = Cast<UFactoryDevUI>(FactoryWidget);
	}

	// UI for 1x1 Factories
	if (FactoryInfoWidget)
	{
		FactoryInfoWidget->SetHeaderText(GetActorNameOrLabel());
		FactoryInfoWidget->SetFactoryDescriptionText("a gnome was here");
		FactoryInfoWidget->SetFactoryInfoText("he was here too");
		FactoryInfoWidget->PopulateQubits(GetNumQubitSlots());
	}

	// UI for Dev Menu Cube
	if (FactoryDevWidget)
	{
		FactoryDevWidget->SetHeaderText("Dev Menu");
	}
	
}

// Called every frame
void AFactoryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FactoryDisplayWindow->IsVisible())
	{
		RotateUIToCamera();
	}
}

void AFactoryBase::RotateUIToCamera()
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FRotator RotationTowardCamera = UKismetMathLibrary::FindLookAtRotation(
		FactoryDisplayWindow->GetComponentLocation() ,
		CameraManager->GetTransformComponent()->GetComponentLocation());
	//FRotator RotationTowardCamera = UKismetMathLibrary::NegateRotator(CameraManager->GetCameraRotation());
	FactoryDisplayWindow->SetWorldRotation(RotationTowardCamera);
}

// set the linked qubits of each QubitDisplaySlot
void AFactoryBase::UpdateQubitDisplay()
{
	if (FactoryWidget) {
		UFactoryInfoUI* FactoryInfoWidget = Cast<UFactoryInfoUI>(FactoryWidget);
		if (FactoryInfoWidget)
		{
			for (int i = 0; i < GetNumQubitSlots(); i++)
			{
				FactoryInfoWidget->UpdateQubit(i, *Qubits[i]);
			}
		}
	}
	
}

void AFactoryBase::OutputQubits()
{
	//Get output factory and send qubits
	int32 SlotNumber = 0;
	for (UFactoryOutputComponent* CurrentOutputComponent : OutputComponents)
	{
		if (CurrentOutputComponent->OutputSlot)
		{
			AActor* CurrentActor = CurrentOutputComponent->OutputSlot;
			if (CurrentActor)
			{
				//UE_LOG(LogTemp, Display, TEXT("Found Actor to send Qubit: %s"), *CurrentActor->GetActorNameOrLabel());
				IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(CurrentOutputComponent->OutputSlot);
				if (IOInterface)
				{
					// Need a way to get the slot index from other actor and then use it here
					UFactoryInputComponent* ConnectedInputComponent = IOInterface->GetConnectedInputComponent(CurrentOutputComponent);
					if (ConnectedInputComponent)
					{
						int32 InputSlotIndex = ConnectedInputComponent->SlotIndex;
						//UE_LOG(LogTemp, Display, TEXT("The input comp of %s has a slot index of %d"), *ConnectedInputComponent->GetOwner()->GetActorNameOrLabel(), InputSlotIndex);
						if (IOInterface->IsQubitSlotEmpty(InputSlotIndex))
						{
							IOInterface->TransferQubit(Qubits[SlotNumber], InputSlotIndex);
							Qubits[SlotNumber] = nullptr;
							UpdateQubitDisplay();
						}
					}
				}
			}

		}
		SlotNumber++;
	}
}

void AFactoryBase::OnFactoryTick()
{
	//UE_LOG(LogTemp, Display, TEXT("Running On Facory tick in Actor: %s"), *GetActorNameOrLabel());
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
	//UE_LOG(LogTemp, Display, TEXT("Clicked on %s"), *GetActorNameOrLabel());
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	switch (PlacementMode)
	{
		case EPlacementMode::Disabled:
			//UE_LOG(LogTemp, Display, TEXT("Selecting Actor %s"), *GetActorNameOrLabel());
			FactoryDisplayWindow->ToggleVisibility();
			UpdateQubitDisplay();

			// Open selected pop up UI
			break;
		case EPlacementMode::Placing:

			break;
		case EPlacementMode::Editing:
		{
			//UE_LOG(LogTemp, Display, TEXT("Picking up Actor %s"), *GetActorNameOrLabel());
			// Remove from collision map
			TArray<FGridCoordinate> GridLocations = GridPlacement->GridComponentToCoordinates(GridPlacementComponent);
			GridPlacement->SetPlacedPositionMap(GridLocations, GridPlacementComponent->GetFactoryShape(), nullptr);

			GridPlacement->PickupFactory(this);
		} break;
		case EPlacementMode::Deletion:
		{
			//UE_LOG(LogTemp, Display, TEXT("Deleting Actor %s"), *GetActorNameOrLabel());
			// Update Collision and then delete
			TArray<FGridCoordinate> GridLocations = GridPlacement->GridComponentToCoordinates(GridPlacementComponent);
			GridPlacement->SetPlacedPositionMap(GridLocations, GridPlacementComponent->GetFactoryShape(), nullptr);
			Destroy();
		} break;
	}
}

// Input Output Interface

void AFactoryBase::SetAllInputOutputsVisibility(bool isVisible)
{
	for (UFactoryInputComponent* InputComp : InputComponents)
	{
		InputComp->SetMeshVisibility(isVisible);
	}

	for (UFactoryOutputComponent* OutputComp : OutputComponents)
	{
		OutputComp->SetMeshVisibility(isVisible);
	}
}

TArray<UFactoryInputComponent*> AFactoryBase::GetInputComponents()
{
	return InputComponents;
}

TArray<UFactoryOutputComponent*> AFactoryBase::GetOutputComponents()
{
	return OutputComponents;
}

void AFactoryBase::ConnectAllInputsAndOutputs()
{
	//UE_LOG(LogTemp, Display, TEXT("ConnectAllInputsAndOutputs Running in %s"), *GetActorNameOrLabel());
}

bool AFactoryBase::IsQubitSlotEmpty(int32 QubitSlotIndex)
{
	if (Qubits[QubitSlotIndex] == nullptr)
	{
		return true;
	}
	return false;
}

void AFactoryBase::TransferQubit(AQubit* QubitToTransfer, int32 QubitSlotIndex)
{
	Qubits[QubitSlotIndex] = QubitToTransfer;
	UpdateQubitDisplay();
}

void AFactoryBase::ConnectAllInputs()
{
	//UE_LOG(LogTemp, Display, TEXT("ConnectAllInputs Running in %s"), *GetActorNameOrLabel());
}

void AFactoryBase::ConnectAllOutputs()
{
	//UE_LOG(LogTemp, Display, TEXT("ConnectAllOutputs Running in %s"), *GetActorNameOrLabel());
}
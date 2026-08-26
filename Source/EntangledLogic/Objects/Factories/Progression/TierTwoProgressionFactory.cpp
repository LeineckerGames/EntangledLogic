#include "TierTwoProgressionFactory.h"

#include "TierOneProgressionFactory.h"
#include "Components/WidgetComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"
#include "EntangledLogic/Objects/Qubits/QubitPreview.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Core/Framework/QubitDataStructs.h"
#include "EntangledLogic/UI/Factory/FactoryProgressionUI.h"


void ATierTwoProgressionFactory::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World && QubitPreviewClass)
	{
		FVector SpawnLocation = GetActorLocation() + QubitPreviewOffset;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParams;
		QubitPreview = GetWorld()->SpawnActor<AQubitPreview>(QubitPreviewClass, SpawnLocation, SpawnRotation, SpawnParams);
		QubitPreview->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

		FKetWrapper ZeroState;
		ZeroState.QubitsInSystem = 1;
		ZeroState.ComplexNumArr.Add(FComplexNumber(1));
		ZeroState.ComplexNumArr.Add(FComplexNumber(0));
		QubitPreview->State = ZeroState;

		UFactorySubsystem* FactorySubsytem = GetWorld()->GetSubsystem<UFactorySubsystem>();
		if (FactorySubsytem)
		{
			FactorySubsytem->UpdateProgressionUIs.AddUObject(this, &ATierTwoProgressionFactory::UpdateQubitPreview);
		}
	}
}

void ATierTwoProgressionFactory::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		// Get the current actor pointers
		AActor* InputSlot0Actor = InputComponents[0]->InputSlot;
		AActor* InputSlot1Actor = InputComponents[1]->InputSlot;
		AActor* InputSlot2Actor = InputComponents[2]->InputSlot;
		AActor* InputSlot3Actor = InputComponents[3]->InputSlot;
		AActor* InputSlot4Actor = InputComponents[4]->InputSlot;
		AActor* InputSlot5Actor = InputComponents[5]->InputSlot;
		AActor* InputSlot6Actor = InputComponents[6]->InputSlot;
		AActor* InputSlot7Actor = InputComponents[7]->InputSlot;


		// Update the prev and next factory pointers
		if (InputSlot0Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot0Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

		if (InputSlot1Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot1Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

		if (InputSlot2Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot2Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

		if (InputSlot3Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot3Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

		if (InputSlot4Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot4Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

		if (InputSlot5Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot5Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

		if (InputSlot6Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot6Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

		if (InputSlot7Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot7Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

		if (QubitPreview)
		{
			QubitPreview->Destroy();
		}

	}
	Super::EndPlay(EndPlayReason);
}

void ATierTwoProgressionFactory::OnFactoryTick()
{
	Super::OnFactoryTick();

}

void ATierTwoProgressionFactory::StartProcessingQubits()
{
	UFactorySubsystem* FactorySubsytem = GetWorld()->GetSubsystem<UFactorySubsystem>();
	UQubitDataSubsystem* QubitSubsystem = GetWorld()->GetSubsystem<UQubitDataSubsystem>();
	if (FactorySubsytem && QubitSubsystem)
	{
		int32 Count = 0;
		for (int i = 0; i < GetNumQubitSlots(); i++)
		{
			if (Qubits[i] != nullptr)
			{
				int32 NumOfProgressionGoals = FactorySubsytem->PersistantStats.CurrentProgressionGoals.Num();
				UE_LOG(LogTemp, Display, TEXT("NumOfProgressionGoals = %d"), NumOfProgressionGoals);
				for (int j = 0; j < NumOfProgressionGoals; j++)
				{

					bool IsQubitEqual = Qubits[i]->State->StateVector.isApprox(FactorySubsytem->PersistantStats.CurrentProgressionGoals[j].GetRequiredKet(), 0.0001);
					UE_LOG(LogTemp, Display, TEXT("IsQubitEqual for qubit #%d = %d"), i, IsQubitEqual);
					if (IsQubitEqual)
					{
						FactorySubsytem->SetProgressionGoalCount(FactorySubsytem->PersistantStats.CurrentProgressionGoals[j],
							FactorySubsytem->PersistantStats.CurrentProgressionGoals[j].ProgressionGoalCount + 1);
					}
					NumOfProgressionGoals = FactorySubsytem->PersistantStats.CurrentProgressionGoals.Num();
				}
			}
			// Delete Qubit
			QubitSubsystem->DeleteQubit(*Qubits[i]);
			Qubits[i] = nullptr;
		}
		//UpdateProgressionUI();
	}
}

void ATierTwoProgressionFactory::UpdateQubitPreview(int32 GoalIndex)
{
	UFactorySubsystem* FactorySubsytem = GetWorld()->GetSubsystem<UFactorySubsystem>();
	if (FactorySubsytem && QubitPreview)
	{
		if (FactorySubsytem->PersistantStats.CurrentProgressionGoals.Num() > 0)
		{
			FProgressionGoal CurrentGoal = FactorySubsytem->PersistantStats.CurrentProgressionGoals[GoalIndex];
			QubitPreview->State = CurrentGoal.ProgressionGoalsData.AcceptedState;
			QubitPreview->UpdateMeshData();
		}
	}
}

void ATierTwoProgressionFactory::OnLoadCompleted()
{
	Super::OnLoadCompleted();
	if (QubitPreview)
	{
		QubitPreview->SetActorLocation(GetActorLocation() + QubitPreviewOffset);
	}
}

// Input Output Interface
void ATierTwoProgressionFactory::ConnectAllInputsAndOutputs()
{
	Super::ConnectAllInputsAndOutputs();

	for (UFactoryInputComponent* CurrentInputComponent : InputComponents)
	{
		ConnectInputComponent(CurrentInputComponent, true);
	}

	for (UFactoryOutputComponent* CurrentOutputComponent : OutputComponents)
	{
		ConnectOutputComponent(CurrentOutputComponent, true);
	}
}

void ATierTwoProgressionFactory::ConnectAllInputs()
{
	Super::ConnectAllInputs();

	for (UFactoryInputComponent* CurrentInputComponent : InputComponents)
	{
		ConnectInputComponent(CurrentInputComponent, false);
	}
}

void ATierTwoProgressionFactory::ConnectAllOutputs()
{
	// Factory Has No Outputs
}
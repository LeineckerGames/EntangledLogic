#include "EntangledLogic/UI/Factory/QubitDisplaySlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "EntangledLogic/Core/Framework/QuantumGatesEnum.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Core/Framework/QubitDataStructs.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"

void UQubitDisplaySlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ZeroStateButton)   ZeroStateButton->OnClicked.AddDynamic(this, &UQubitDisplaySlot::SetStateZero);
	if (OneStateButton)     OneStateButton->OnClicked.AddDynamic(this, &UQubitDisplaySlot::SetStateOne);
	if (PlusStateButton)   PlusStateButton->OnClicked.AddDynamic(this, &UQubitDisplaySlot::SetStatePlus);
	if (MinusStateButton) MinusStateButton->OnClicked.AddDynamic(this, &UQubitDisplaySlot::SetStateMinus);

	if (XGateButton) XGateButton->OnClicked.AddDynamic(this, &UQubitDisplaySlot::ApplyGateX);
	if (YGateButton) YGateButton->OnClicked.AddDynamic(this, &UQubitDisplaySlot::ApplyGateY);
	if (ZGateButton) ZGateButton->OnClicked.AddDynamic(this, &UQubitDisplaySlot::ApplyGateZ);
	if (HGateButton) HGateButton->OnClicked.AddDynamic(this, &UQubitDisplaySlot::ApplyGateH);

	if (NewQubitButton) NewQubitButton->OnClicked.AddDynamic(this, &UQubitDisplaySlot::CreateQubit);

	UWorld* world = GetWorld();
	if (world)
	{
		QubitSubsystem = GetWorld()->GetSubsystem<UQubitDataSubsystem>();
	}
	UpdateStateDisplay();
}

void UQubitDisplaySlot::SetSlotName(FString name)
{
	if (QubitName)
	{
		QubitName->SetText(FText::FromString(name));
	}
}

// creates a qubit in UI, for testing
void UQubitDisplaySlot::CreateQubit()
{
	if (QubitSubsystem) Q = QubitSubsystem->NewQubit();
	UpdateStateDisplay();
}

// set the qubit this slot is displaying the state of
void UQubitDisplaySlot::SetQubit(AQubit& newQubit)
{
	Q = &newQubit;
	UpdateStateDisplay();
}

// set the state of the linked qubit
void UQubitDisplaySlot::SetState(ENamedState state)
{
	if (QubitSubsystem)
	{
		QubitSubsystem->SetState(*Q, state);
	}
	UpdateStateDisplay();
}

// apply a gate to the linked qubit (for testing)
void UQubitDisplaySlot::ApplyGate(EQuantumGate gate)
{
	if (QubitSubsystem)
	{
		QubitSubsystem->Apply(*Q, gate);
	}
	UpdateStateDisplay();
}

void UQubitDisplaySlot::UpdateStateDisplay()
{
	if (Q)
	{
		if (StateText) StateText->SetText(FText::FromString(Q->GetString()));
		if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(1);
	}
	else
	{
		if (StateText) StateText->SetText(FText::FromString(FString("Empty")));
		if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}


void UQubitDisplaySlot::SetStateZero() { SetState(ENamedState::Zero); }
void UQubitDisplaySlot::SetStateOne() { SetState(ENamedState::One); }
void UQubitDisplaySlot::SetStatePlus() { SetState(ENamedState::Plus); }
void UQubitDisplaySlot::SetStateMinus() { SetState(ENamedState::Minus); }

void UQubitDisplaySlot::ApplyGateX() { ApplyGate(EQuantumGate::X_Gate); }
void UQubitDisplaySlot::ApplyGateY() { ApplyGate(EQuantumGate::Y_Gate); }
void UQubitDisplaySlot::ApplyGateZ() { ApplyGate(EQuantumGate::Z_Gate); }
void UQubitDisplaySlot::ApplyGateH() { ApplyGate(EQuantumGate::H_Gate); }
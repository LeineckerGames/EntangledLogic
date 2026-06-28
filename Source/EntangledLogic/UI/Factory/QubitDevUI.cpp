#include "EntangledLogic/UI/Factory/QubitDevUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "EntangledLogic/Core/Framework/QuantumGatesEnum.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"

void UQubitDevUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ZeroStateAButton)   ZeroStateAButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateAZero);
	if (ZeroStateBButton)   ZeroStateBButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateBZero);
	if (OneStateAButton)     OneStateAButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateAOne);
	if (OneStateBButton)     OneStateBButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateBOne);
	if (PlusStateAButton)   PlusStateAButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateAPlus);
	if (PlusStateBButton)   PlusStateBButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateBPlus);
	if (MinusStateAButton) MinusStateAButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateAMinus);
	if (MinusStateBButton) MinusStateBButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateBMinus);
	if (BellStateButton)     BellStateButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateBell);

	if (IGateAButton) IGateAButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateIA);
	if (IGateBButton) IGateBButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateIB);
	if (XGateAButton) XGateAButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateXA);
	if (XGateBButton) XGateBButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateXB);
	if (YGateAButton) YGateAButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateYA);
	if (YGateBButton) YGateBButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateYB);
	if (ZGateAButton) ZGateAButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateZA);
	if (ZGateBButton) ZGateBButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateZB);
	if (HGateAButton) HGateAButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateHA);
	if (HGateBButton) HGateBButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateHB);
	if (CNOTGateAButton) CNOTGateAButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateCNOTA);
	if (CNOTGateBButton) CNOTGateBButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateCNOTB);

	UWorld* world = GetWorld();
	if (world)
	{
		QubitSubsystem = GetWorld()->GetSubsystem<UQubitDataSubsystem>();
		if (QubitSubsystem)
		{
			QA = QubitSubsystem->NewQubit();
			QB = QubitSubsystem->NewQubit();
			SetStateText();
		}
	}
}

void UQubitDevUI::SetState(ENamedState state, bool Qslot)
{
	if (QubitSubsystem)
	{
		QubitSubsystem->SetState((Qslot ? *QB : *QA), state);
	}
	SetStateText();
}

void UQubitDevUI::ApplyGate(EOneQubitGate gate, bool Qslot)
{
	if (QubitSubsystem)
	{
		QubitSubsystem->Apply((Qslot ? *QB : *QA), gate);
	}
	SetStateText();
}

void UQubitDevUI::ApplyGate2(ETwoQubitGate gate, bool Qslot)
{
	return;
}

void UQubitDevUI::SetStateText()
{
	FText TextA = FText::FromString(QA->GetString());
	FText TextB = FText::FromString(QB->GetString());

	// naive check
	if (TextA.EqualTo(TextB))
	{
		StateText->SetText(TextA);
		StateAText->SetVisibility(ESlateVisibility::Collapsed);
		StateBText->SetVisibility(ESlateVisibility::Collapsed);
		StateText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		StateAText->SetText(TextA);
		StateBText->SetText(TextB);
		StateText->SetVisibility(ESlateVisibility::Collapsed);
		StateAText->SetVisibility(ESlateVisibility::Visible);
		StateBText->SetVisibility(ESlateVisibility::Visible);
	}
}


void UQubitDevUI::SetStateBell()
{
	return;
}

void UQubitDevUI::SetStateAZero() { SetState(ENamedState::Zero, false); }
void UQubitDevUI::SetStateBZero() { SetState(ENamedState::Zero, true); }
void UQubitDevUI::SetStateAOne()  { SetState(ENamedState::One, false); }
void UQubitDevUI::SetStateBOne()  { SetState(ENamedState::One, true); }
void UQubitDevUI::SetStateAPlus() { SetState(ENamedState::Plus, false); }
void UQubitDevUI::SetStateBPlus() { SetState(ENamedState::Plus, true); }
void UQubitDevUI::SetStateAMinus() { SetState(ENamedState::Minus, false); }
void UQubitDevUI::SetStateBMinus() { SetState(ENamedState::Minus, true); }


void UQubitDevUI::ApplyGateIA() { ApplyGate(EOneQubitGate::Identity, false); }
void UQubitDevUI::ApplyGateIB() { ApplyGate(EOneQubitGate::Identity, true); }
void UQubitDevUI::ApplyGateXA() { ApplyGate(EOneQubitGate::X_Gate, false); }
void UQubitDevUI::ApplyGateXB() { ApplyGate(EOneQubitGate::X_Gate, true); }
void UQubitDevUI::ApplyGateYA() { ApplyGate(EOneQubitGate::Y_Gate, false); }
void UQubitDevUI::ApplyGateYB() { ApplyGate(EOneQubitGate::Y_Gate, true); }
void UQubitDevUI::ApplyGateZA() { ApplyGate(EOneQubitGate::Z_Gate, false); }
void UQubitDevUI::ApplyGateZB() { ApplyGate(EOneQubitGate::Z_Gate, true); }
void UQubitDevUI::ApplyGateHA() { ApplyGate(EOneQubitGate::H_Gate, false); }
void UQubitDevUI::ApplyGateHB() { ApplyGate(EOneQubitGate::H_Gate, true); }
void UQubitDevUI::ApplyGateCNOTA() { ApplyGate2(ETwoQubitGate::CNOT_Gate, false); }
void UQubitDevUI::ApplyGateCNOTB() { ApplyGate2(ETwoQubitGate::CNOT_Gate, true); }
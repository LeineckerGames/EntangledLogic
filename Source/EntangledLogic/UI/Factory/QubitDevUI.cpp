
#include "EntangledLogic/UI/Factory/QubitDevUI.h"
#include "Components/Button.h"
#include "EntangledLogic/Core/Framework/QuantumGatesEnum.h"

void UQubitDevUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ZeroStateButton)   ZeroStateButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateZero);
	if (OneStateButton)     OneStateButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateOne);
	if (PlusStateButton)   PlusStateButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStatePlus);
	if (MinusStateButton) MinusStateButton->OnClicked.AddDynamic(this, &UQubitDevUI::SetStateMinus);

	if (IGateButton) IGateButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateI);
	if (XGateButton) XGateButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateX);
	if (YGateButton) YGateButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateY);
	if (ZGateButton) ZGateButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateZ);
	if (HGateButton) HGateButton->OnClicked.AddDynamic(this, &UQubitDevUI::ApplyGateH);
}

void UQubitDevUI::SetState(ENamedState state)
{
	return;
}

void UQubitDevUI::ApplyGate(EOneQubitGate gate)
{
	return;
}

void UQubitDevUI::SetStateZero()  { SetState(ENamedState::Zero); }
void UQubitDevUI::SetStateOne()   { SetState(ENamedState::One); }
void UQubitDevUI::SetStatePlus()  { SetState(ENamedState::Plus); }
void UQubitDevUI::SetStateMinus() { SetState(ENamedState::Minus); }

void UQubitDevUI::ApplyGateI() { ApplyGate(EOneQubitGate::Identity); }
void UQubitDevUI::ApplyGateX() { ApplyGate(EOneQubitGate::X_Gate); }
void UQubitDevUI::ApplyGateY() { ApplyGate(EOneQubitGate::Y_Gate); }
void UQubitDevUI::ApplyGateZ() { ApplyGate(EOneQubitGate::Z_Gate); }
void UQubitDevUI::ApplyGateH() { ApplyGate(EOneQubitGate::H_Gate); }
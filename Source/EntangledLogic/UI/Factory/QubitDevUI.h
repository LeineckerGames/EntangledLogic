#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QubitDevUI.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API UQubitDevUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class AQubit* QA;

	UPROPERTY()
	class AQubit* QB;

	UPROPERTY()
	class UQubitDataSubsystem* QubitSubsystem;

	UPROPERTY(meta = (BindWidget))
	class UFactoryUIBase* UIBase;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StateText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StateAText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StateBText;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetState(ENamedState state, bool Qslot);

	UFUNCTION()
	void ApplyGate(EOneQubitGate gate, bool Qslot);

	UFUNCTION()
	void ApplyGate2(ETwoQubitGate gate, bool Qslot);

	UFUNCTION()
	void SetStateText();

	// go no further, for your own safety

	UPROPERTY(meta = (BindWidget))
	class UButton* ZeroStateAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ZeroStateBButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OneStateAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OneStateBButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlusStateAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlusStateBButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MinusStateAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MinusStateBButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BellStateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* IGateAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* IGateBButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* XGateAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* XGateBButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* YGateAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* YGateBButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ZGateAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ZGateBButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HGateAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HGateBButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CNOTGateAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CNOTGateBButton;

	UFUNCTION()
	void SetStateAZero();

	UFUNCTION()
	void SetStateBZero();

	UFUNCTION()
	void SetStateAOne();

	UFUNCTION()
	void SetStateBOne();

	UFUNCTION()
	void SetStateAPlus();

	UFUNCTION()
	void SetStateBPlus();

	UFUNCTION()
	void SetStateAMinus();

	UFUNCTION()
	void SetStateBMinus();

	UFUNCTION()
	void SetStateBell();

	UFUNCTION()
	void ApplyGateIA();

	UFUNCTION()
	void ApplyGateIB();

	UFUNCTION()
	void ApplyGateXA();

	UFUNCTION()
	void ApplyGateXB();

	UFUNCTION()
	void ApplyGateYA();

	UFUNCTION()
	void ApplyGateYB();

	UFUNCTION()
	void ApplyGateZA();

	UFUNCTION()
	void ApplyGateZB();

	UFUNCTION()
	void ApplyGateHA();

	UFUNCTION()
	void ApplyGateHB();

	UFUNCTION()
	void ApplyGateCNOTA();

	UFUNCTION()
	void ApplyGateCNOTB();
};



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
	class AQubit* Q;

	UPROPERTY()
	class UQubitDataSubsystem* QubitSubsystem;

	UPROPERTY(meta = (BindWidget))
	class UFactoryUIBase* UIBase;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StateText;

	UPROPERTY(meta = (BindWidget))
	class UButton* ZeroStateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OneStateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlusStateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MinusStateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* IGateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* XGateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* YGateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ZGateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HGateButton;


	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetState(ENamedState state);

	UFUNCTION()
	void ApplyGate(EOneQubitGate gate);

	UFUNCTION()
	void SetStateText();

	UFUNCTION()
	void SetStateZero();

	UFUNCTION()
	void SetStateOne();

	UFUNCTION()
	void SetStatePlus();

	UFUNCTION()
	void SetStateMinus();

	UFUNCTION()
	void ApplyGateI();

	UFUNCTION()
	void ApplyGateX();

	UFUNCTION()
	void ApplyGateY();

	UFUNCTION()
	void ApplyGateZ();

	UFUNCTION()
	void ApplyGateH();
};

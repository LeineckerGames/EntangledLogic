#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QubitDisplaySlot.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API UQubitDisplaySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	class AQubit* Q;

	UPROPERTY()
	class UQubitDataSubsystem* QubitSubsystem;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StateText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QubitName;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetSlotName(FString name);

	//UFUNCTION()  // unreal doesn't like this for some reason
	void SetQubit(AQubit& newQubit);

	UFUNCTION()
	void UpdateStateDisplay();

protected:
	UFUNCTION()
	void CreateQubit();

	UFUNCTION()
	void SetState(ENamedState state);

	UFUNCTION()
	void ApplyGate(EQuantumGate gate);


	UPROPERTY(meta = (BindWidget))
	class UButton* NewQubitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ZeroStateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OneStateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlusStateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MinusStateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* XGateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* YGateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ZGateButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HGateButton;

	UFUNCTION()
	void SetStateZero();

	UFUNCTION()
	void SetStateOne();

	UFUNCTION()
	void SetStatePlus();

	UFUNCTION()
	void SetStateMinus();

	UFUNCTION()
	void ApplyGateX();

	UFUNCTION()
	void ApplyGateY();

	UFUNCTION()
	void ApplyGateZ();

	UFUNCTION()
	void ApplyGateH();
};

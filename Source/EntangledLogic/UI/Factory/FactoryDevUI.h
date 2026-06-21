#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FactoryDevUI.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API UFactoryDevUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UFactoryUIBase* UIBase;

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* DeleteSaveButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ClearFactoriesButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* TickPauseButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* TickPlayButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextTickButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* PlayPauseSwitch;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void LoadGame();

	UFUNCTION()
	void DeleteSave();

	UFUNCTION()
	void ClearFactories();

	UFUNCTION()
	void ToggleFactoryTick();

	UFUNCTION()
	void NextFactoryTick();

	UFUNCTION()
	void UnlockRealQuantumGnome();


public:
	void SetHeaderText(FString FactoryHeader);

};

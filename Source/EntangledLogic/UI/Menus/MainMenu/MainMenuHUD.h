#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class UMainMenuWidget;
class USettingsMenuWidget;

UCLASS()
class ENTANGLEDLOGIC_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USettingsMenuWidget> SettingsMenuWidgetClass;
private:

	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

	UPROPERTY()
	USettingsMenuWidget* SettingsMenuWidget;

	UFUNCTION()
	void OpenSettingsMenu();

	UFUNCTION()
	void CloseSettingsMenu();

	UFUNCTION()
	void QuitToDesktop();
	*/
};
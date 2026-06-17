#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UFactorySelectionWidget;

UCLASS()
class ENTANGLEDLOGIC_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UFactorySelectionWidget> FactorySelectionWidgetClass;

	APlayerHUD();

	void RepopulateFactorySelectionWidget();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UFactorySelectionWidget* FactorySelectionWidget;
	
};

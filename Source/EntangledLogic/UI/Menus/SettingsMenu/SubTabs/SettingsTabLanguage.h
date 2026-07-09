#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsTabLanguage.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API USettingsTabLanguage : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
};
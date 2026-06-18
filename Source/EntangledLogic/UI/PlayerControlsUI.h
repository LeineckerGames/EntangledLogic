#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerControlsUI.generated.h"

struct FControlData;
class UControlBase;

UCLASS()
class ENTANGLEDLOGIC_API UPlayerControlsUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UKeyboardIconKeymappingsData* KeyboardIconKeymappingData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* BlankKeyIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UControlBase> ControlBaseClass;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ControlVerticalBox;

	UControlBase* CreateControlUIFromControlData(FControlData ControlData);

public:

	void UpdatePlayerControlsUI();

};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlBase.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API UControlBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ControlText;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* IconBox;


public:

	void AddIconToUI(UTexture2D* IconToAdd);

	void SetControlText(FString TextToSet);

	void SetControlText(FText TextToSet);

};

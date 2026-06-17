#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FactoryInfoUI.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API UFactoryInfoUI : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FactoryDescriptionText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FactoryInfoText;

	UPROPERTY(meta = (BindWidget))
	class UFactoryUIBase* UIBase;

public:

	void SetFactoryDescriptionText(FString FactoryDescription);

	void SetFactoryInfoText(FString FactoryInfo);

	void SetHeaderText(FString FactoryHeader);

	bool GetIsPinned() const;
}; 

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

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* QubitDisplayBox;

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UQubitDisplaySlot> QubitDisplaySlotClass;

	void SetFactoryDescriptionText(FString FactoryDescription);

	void SetFactoryInfoText(FString FactoryInfo);

	void SetHeaderText(FString FactoryHeader);

	void PopulateQubits(int32 NumQubits);

	void UpdateQubit(int32 id, class AQubit &Q);

	bool GetIsPinned() const;
}; 

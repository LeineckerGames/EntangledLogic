#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FactoryProgressionUI.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API UFactoryProgressionUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProgressionCountText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QubitStateGoalText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentGoalText;

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateProgressionUI(int32 ProgressionGoalIndex);

};

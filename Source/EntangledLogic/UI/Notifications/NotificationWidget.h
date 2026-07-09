#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationWidget.generated.h"


UCLASS()
class ENTANGLEDLOGIC_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	UPROPERTY(meta = (BindWidget))
	class UOverlay* NotificationOverlay;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* NotifcationSizeBox;

	//UPROPERTY(meta = (BindWidget))
	//class UTextBox* NotificationText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* NotificationText;

};

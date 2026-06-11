
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownPlayerController();

	UPROPERTY(EditDefaultsOnly);
	class UInputMappingContext* PlayerControls;

	UPROPERTY(EditDefaultsOnly);
	class UInputMappingContext* GridControls;
	
	void AddMappingContext(UInputMappingContext* InputMappingContext, int32 Priority);

	void RemoveMappingContext(UInputMappingContext* PlayerControls);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

};

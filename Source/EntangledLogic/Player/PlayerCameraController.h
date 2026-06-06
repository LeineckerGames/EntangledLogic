
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputLibrary.h"
#include "PlayerCameraController.generated.h"

struct FInputActionValue;

UCLASS()
class ENTANGLEDLOGIC_API APlayerCameraController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCameraController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Attached Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFloatingPawnMovement* FloatingPawnMovement;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* KeyboardMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* DragMovement;

};

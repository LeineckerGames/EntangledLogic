
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputLibrary.h"
#include "Math/Vector2D.h"
#include "PlayerCameraController.generated.h"

struct FInputActionValue;
enum class EPlacementMode : uint8;
UCLASS()
class ENTANGLEDLOGIC_API APlayerCameraController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCameraController();

private:
	bool isDragging = false;
	bool isRotating = false;
	float DragSensitivity = 50.0f;
	int32 RotateSensitivity = 2;
	int32 ZoomSpeed = 50;
	float ZoomSensitivityMultiplier = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class APlayerController* TopDownPlayerController;

	class UGridPlacementSubsystem* GridPlacement;

	void Move(const FInputActionValue& Value);

	void DragMove(const FInputActionValue& Value);

	void ZoomCamera(const FInputActionValue& Value);

	void RotateCamera(const FInputActionValue& Value);

	void OnLeftClick(const FInputActionValue& Value);

	void OnLeftClickCompleted(const FInputActionValue& Value);

	void OnRightClick(const FInputActionValue& Value);

	void OnRightClickCompleted(const FInputActionValue& Value);

	void OnPauseAction(const FInputActionValue& Value);

	FVector GetWorldMousePosition();

	void OnPlacementModeChanged(EPlacementMode CurrentPlacementMode);

	void OutlineHoveredFactory();

	class IFactoryInteractionInterface* PreviousInteraction;

	UPROPERTY(EditAnywhere, Category = "Background Music")
	class USoundBase* BackgroundMusic;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class AActor* GetHoveredActorFromMousePosition();

	class IFactoryInteractionInterface* GetIFactoryInteractionFromMouse();

	// Attached Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetInteractionComponent* WidgetInteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* GridPlane;

	UPROPERTY(EditAnywhere, Category = "Grid Material")
	class UMaterialInterface* GridMaterial;

	class UMaterialInstanceDynamic* GridMaterialInstance;

	// Player Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* KeyboardMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* DragMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* PlayerLeftClick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* PlayerRightClick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* Zoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* Rotate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* PauseAction;

	// Grid Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* CancelPlacement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* DeletionMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* EditingMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* GridLeftClick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* RotateFactory;

};

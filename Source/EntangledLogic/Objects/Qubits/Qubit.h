#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Qubit.generated.h"

struct FQubitData;

UCLASS()
class ENTANGLEDLOGIC_API AQubit : public AActor
{
	GENERATED_BODY()
	
public:
	
	class TSharedRef<FQubitData> State = MakeShared<FQubitData>();
	int32 EntanglementPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* QubitMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* EntanglementSplineMesh;

	UPROPERTY()
	TArray<class USplineComponent*> EntanglementSplines;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* QubitNiagaraComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* QubitEntanglementFX;

	// Sets default values for this actor's properties
	AQubit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool UpdateSplineMovement = false;

	FVector SplineOffset = FVector(0.0f, 0.0f, 7.0f);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginCursorOver() override;

	virtual void NotifyActorEndCursorOver() override;

	class FString GetString();

	void UpdateMeshData();

	void AttachEntanglementSplineToQubit(AQubit* QubitToAttach);

	void UpdateEntanglementSplinePointLocation(FVector EndSplinePointLocation, int32 SplineIndex);

	void SetEntanglementSplineVisibility(bool VisibilityToSet);

	void ToggleEntanglementFX(bool ValueToSet);
};

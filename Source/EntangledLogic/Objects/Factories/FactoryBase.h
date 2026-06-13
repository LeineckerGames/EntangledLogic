#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FactoryBase.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API AFactoryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFactoryBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere)
	class UGridPlacementComponent* GridPlacementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* FactoryMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

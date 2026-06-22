

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Qubit.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API AQubit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQubit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

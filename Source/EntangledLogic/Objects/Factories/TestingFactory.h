

#pragma once

#include "CoreMinimal.h"
#include "FactoryBase.h"
#include "TestingFactory.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API ATestingFactory : public AFactoryBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestingFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};

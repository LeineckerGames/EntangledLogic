#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FactorySubsystem.generated.h"


UCLASS()
class ENTANGLEDLOGIC_API UFactorySubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
protected:

	bool CanTick = false;

	FTimerManager TimerManager;

public:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	void SetTickTrue();

};

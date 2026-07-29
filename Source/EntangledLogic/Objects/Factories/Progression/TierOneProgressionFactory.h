#pragma once

#include "CoreMinimal.h"
#include "../FactoryBase.h"
#include "TierOneProgressionFactory.generated.h"


UCLASS()
class ENTANGLEDLOGIC_API ATierOneProgressionFactory : public AFactoryBase
{
	GENERATED_BODY()

	static constexpr int32 NUM_QUBIT_SLOTS = 4;

	int32 GetNumQubitSlots() override { return NUM_QUBIT_SLOTS; };
	
	class AQubitPreview* QubitPreview;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AQubitPreview> QubitPreviewClass;

	UPROPERTY(BlueprintReadwrite, EditAnywhere)
	FVector QubitPreviewOffset;

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnFactoryTick() override;

	virtual void StartProcessingQubits() override;

	virtual void OnLoadCompleted() override;

	//void UpdateProgressionUI();

	void UpdateQubitPreview(int32 GoalIndex);

public:

	// Input Output Interface
	virtual void ConnectAllInputsAndOutputs() override;

	virtual void ConnectAllInputs() override;

	virtual void ConnectAllOutputs() override;
};

#pragma once

#include "CoreMinimal.h"
#include "FactoryBase.h"
#include "QuantumGnomeFactory.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API AQuantumGnomeFactory : public AFactoryBase
{
	GENERATED_BODY()

public:
	AQuantumGnomeFactory();

	virtual void Interact(EPlacementMode PlacementMode) override;

protected:

	virtual void BeginPlay() override;

	void Explode();

	int32 ClickCount = 0;

	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* AudioComponenet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UGeometryCollectionComponent* GnomeFracturedGeometry;

};

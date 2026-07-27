

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntangledLogic/Core/Framework/ProgressionGoalsDataStructs.h"
#include "QubitPreview.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API AQubitPreview : public AActor
{
	GENERATED_BODY()
	
public:	

	class TSharedRef<FKetWrapper> State = MakeShared<FKetWrapper>();
	int32 EntanglementPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* QubitMesh;

	// Sets default values for this actor's properties
	AQubitPreview();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

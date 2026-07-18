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

	// Sets default values for this actor's properties
	AQubit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class FString GetString();

	void UpdateMeshData();
};

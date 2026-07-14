#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoalTrackerEntry.generated.h"

UENUM()
enum class EGoalTrackerEntryState : uint8
{
	Preview,
	Collapsed,
	Expanded
};


UCLASS()
class ENTANGLEDLOGIC_API UGoalTrackerEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGoalTrackerEntryState State;
};

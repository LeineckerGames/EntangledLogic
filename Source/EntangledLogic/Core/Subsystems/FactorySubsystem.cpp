#include "FactorySubsystem.h"
#include "TimerManager.h"

void UFactorySubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
	if (UWorld* World = GetWorld())
	{
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle, this, &UFactorySubsystem::SetTickTrue, 2, true);
	}
	
}

void UFactorySubsystem::SetTickTrue()
{
	CanTick = true;
}

void UFactorySubsystem::Tick(float DeltaTime)
{
	if (CanTick)
	{
		CanTick = false;
		UE_LOG(LogTemp, Display, TEXT("FACTORY TICK"));
	}
}

TStatId UFactorySubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFactorySubsystem, STATGROUP_Tickables);
}
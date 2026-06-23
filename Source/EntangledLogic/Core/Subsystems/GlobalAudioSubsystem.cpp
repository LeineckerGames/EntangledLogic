#include "GlobalAudioSubsystem.h"
#include "Kismet/GameplayStatics.h"


void UGlobalAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UGlobalAudioSubsystem::StartBackgroundMusic()
{
	UWorld* World = GetWorld();

	if (BackgroundMusic && World)
	{
		UE_LOG(LogTemp, Display, TEXT("Playing Music"));
		UGameplayStatics::PlaySound2D(World, BackgroundMusic);
	}
}
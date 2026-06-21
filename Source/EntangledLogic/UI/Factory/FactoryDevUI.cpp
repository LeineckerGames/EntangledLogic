#include "FactoryDevUI.h"
#include "FactoryUIBase.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"
#include "EntangledLogic/Core/Subsystems/SavingLoadingSubsystem.h"
#include "EntangledLogic/Core/Framework/UnlockablesEnum.h"

void UFactoryDevUI::NativeConstruct()
{
    Super::NativeConstruct();

    //HeaderText->SetText(FText::FromString("Testing Title"));

    if (SaveButton)
    {
        SaveButton->OnClicked.AddDynamic(this, &UFactoryDevUI::SaveGame);
    }
    if (LoadButton)
    {
        LoadButton->OnClicked.AddDynamic(this, &UFactoryDevUI::LoadGame);
    }
    if (DeleteSaveButton)
    {
        DeleteSaveButton->OnClicked.AddDynamic(this, &UFactoryDevUI::DeleteSave);
    }
    if (ClearFactoriesButton)
    {
        ClearFactoriesButton->OnClicked.AddDynamic(this, &UFactoryDevUI::ClearFactories);
    }
    if (TickPauseButton)
    {
        TickPauseButton->OnClicked.AddDynamic(this, &UFactoryDevUI::ToggleFactoryTick);
    }
    if (TickPlayButton)
    {
        TickPlayButton->OnClicked.AddDynamic(this, &UFactoryDevUI::ToggleFactoryTick);
    }
    if (NextTickButton)
    {
        NextTickButton->OnClicked.AddDynamic(this, &UFactoryDevUI::NextFactoryTick);
    }


}

void UFactoryDevUI::ToggleFactoryTick()
{
    UFactorySubsystem* FactorySubsystem = GetWorld()->GetSubsystem<UFactorySubsystem>();
    // 0 = Pause, 1 = Play
    if (FactorySubsystem->GetTickPaused())
    {
        FactorySubsystem->SetTickPaused(false);
        PlayPauseSwitch->SetActiveWidgetIndex(0);
    }
    else
    {
        FactorySubsystem->SetTickPaused(true);
        PlayPauseSwitch->SetActiveWidgetIndex(1);
    }
}


void UFactoryDevUI::NextFactoryTick()
{
    UFactorySubsystem* FactorySubsystem = GetWorld()->GetSubsystem<UFactorySubsystem>();
    FactorySubsystem->SetCanTick(true);
}

void UFactoryDevUI::SaveGame()
{
    UE_LOG(LogTemp, Display, TEXT("Save Game Pressed"));
    USavingLoadingSubsystem* SavingLoadingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USavingLoadingSubsystem>();
    SavingLoadingSubsystem->RequestSave();
}

void UFactoryDevUI::LoadGame()
{
    UE_LOG(LogTemp, Display, TEXT("Load Game Pressed"));
}

void UFactoryDevUI::DeleteSave()
{
    UE_LOG(LogTemp, Display, TEXT("Delete Save Pressed"));
}

void UFactoryDevUI::ClearFactories()
{
    UE_LOG(LogTemp, Display, TEXT("Clear all Factories Pressed"));
}

void UFactoryDevUI::SetHeaderText(FString FactoryHeader)
{
	UIBase->SetHeaderText(FactoryHeader);
}

void UFactoryDevUI::UnlockRealQuantumGnome()
{
    // temp using this to test the progression
    UFactorySubsystem* FactorySubsystem = GetWorld()->GetSubsystem<UFactorySubsystem>();
    FactorySubsystem->UnlockProgression(EUnlockables::Factory_QuantumGnome);
}
#include "FactoryDevUI.h"
#include "FactoryUIBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

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


}

void UFactoryDevUI::SaveGame()
{
    UE_LOG(LogTemp, Display, TEXT("Save Game Pressed"));
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
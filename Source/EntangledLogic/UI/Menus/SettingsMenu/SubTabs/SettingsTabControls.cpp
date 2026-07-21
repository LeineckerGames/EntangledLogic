#include "SettingsTabControls.h"
#include "Components/ScrollBox.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/Components/ControlRowWidget.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/Data/MappableControlList.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/Data/SettingsDataTypes.h"

void USettingsTabControls::NativeConstruct()
{
    Super::NativeConstruct();
    PopulateControlsList();
}

void USettingsTabControls::PopulateControlsList()
{
    if (!ControlsDataAsset || !ControlRowTemplate || !ControlsContainer) return;

    ControlsContainer->ClearChildren();

    APlayerController* PC = GetOwningPlayer();
    ULocalPlayer* LocalPlayer = PC ? PC->GetLocalPlayer() : nullptr;
    if (!LocalPlayer) return;

    auto* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!InputSubsystem) return;

    UEnhancedInputUserSettings* UserSettings = InputSubsystem->GetUserSettings();
    if (!UserSettings) return;

    UEnhancedPlayerMappableKeyProfile* ActiveProfile = UserSettings->GetActiveKeyProfile();
    if (!ActiveProfile) return;

    for (const FKeyMappingDisplayInfo& RowInfo : ControlsDataAsset->ControlRows)
    {
        if (!RowInfo.TargetAction) continue;

        TArray<FKey> MappedKeys;
        ActiveProfile->GetMappedKeysInRow(RowInfo.TargetAction->GetFName(), MappedKeys);

        FKey PrimaryKey = (MappedKeys.Num() > 0) ? MappedKeys[0] : EKeys::Invalid;
        FKey SecondaryKey = (MappedKeys.Num() > 1) ? MappedKeys[1] : EKeys::Invalid;

        UControlRowWidget* NewRow = CreateWidget<UControlRowWidget>(this, ControlRowTemplate);
        if (NewRow)
        {
            NewRow->InitializeRow(RowInfo.ActionFriendlyName, RowInfo.TargetAction, PrimaryKey, SecondaryKey);
            ControlsContainer->AddChild(NewRow);
        }
    }
}
/*
void USettingsTabControls::InitializeControlsTab()
{
    if (!ControlsListContainer || !ControlsRowWidgetClass || !DefaultMappingContext) return;

    //ControlsListContainer->ClearChildren();

    APlayerController* PC = GetOwningPlayer();
    ULocalPlayer* LocalPlayer = PC ? PC->GetLocalPlayer() : nullptr;
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!InputSubsystem) return;

    const TArray<FEnhancedActionKeyMapping>& AllMappings = DefaultMappingContext->GetMappings();

    for (const FKeyMappingDisplayInfo& ActionInfo : MappableActions)
    {
        if (!ActionInfo.TargetAction) continue;

        FKey BoundKey = EKeys::Invalid;
        for (const FEnhancedActionKeyMapping& Mapping : AllMappings)
        {
            if (Mapping.Action == ActionInfo.TargetAction)
            {
                BoundKey = Mapping.Key;
                break;
            }
        }

        UUserWidget* NewRow = CreateWidget<UUserWidget>(PC, ControlsRowWidgetClass);
        if (NewRow)
        {
            // Update row labels here by casting to your custom row type
            ControlsListContainer->AddChildToVerticalBox(NewRow);
        }
    }
}
*/
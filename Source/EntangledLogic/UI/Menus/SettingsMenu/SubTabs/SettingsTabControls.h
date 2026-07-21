#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/Data/MappableControlList.h"
#include "SettingsTabControls.generated.h"

class UMappableControlList;
class UScrollBox;

UCLASS()
class ENTANGLEDLOGIC_API USettingsTabControls : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // Controls Settings
    UPROPERTY(meta = (BindWidget))
    UScrollBox* ControlsContainer;

    // Expose the Data Asset pointer so we can assign our spreadsheet in the editor
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controls Settings")
    UMappableControlList* ControlsDataAsset;

    // The Subclass template we want to spawn (WBP_ControlRow)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controls Settings")
    TSubclassOf<class UControlRowWidget> ControlRowTemplate;

public:
    // Dedicated function to read data and update UI layout elements
    void PopulateControlsList();
};

/*


        UPROPERTY(EditDefaultsOnly, Category = "Input Configuration")
        UInputMappingContext* DefaultMappingContext;

        UPROPERTY(EditDefaultsOnly, Category = "Input Configuration")
        TSubclassOf<UUserWidget> ControlsRowWidgetClass;

        UPROPERTY(EditDefaultsOnly, Category = "Input Configuration")
        TArray<FKeyMappingDisplayInfo> MappableActions;
*/
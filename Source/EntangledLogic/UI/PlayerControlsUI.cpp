#include "PlayerControlsUI.h"
#include "ControlBase.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "EnhancedActionKeyMapping.h"
#include "InputAction.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Texture2D.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "EntangledLogic/Core/Framework/KeyboardIconKeymappingsData.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"



void UPlayerControlsUI::UpdatePlayerControlsUI()
{
	ControlVerticalBox->ClearChildren();
	TArray<FControlData> ControlList = KeyboardIconKeymappingData->ControlsList;
	// Iterate through KeyboardIconKeymappingData ControlsList Array
	for (FControlData ControlData : ControlList)
	{
		UControlBase* ControlUI = CreateControlUIFromControlData(ControlData);
		UPanelSlot* ControlUIPanelSlot = ControlVerticalBox->AddChild(ControlUI);
		// Set the Alignment to Bottom Left
		UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>(ControlUIPanelSlot);
		if (VerticalBoxSlot)
		{
			VerticalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
			VerticalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
		}
	}
}

UControlBase* UPlayerControlsUI::CreateControlUIFromControlData(FControlData ControlData)
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
		FText* DescriptionText = ControlData.PlacementModeDescriptions.Find(GridPlacement->GetPlacementMode());
		if (DescriptionText)
		{
			if (APlayerController* PC = GetOwningPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
				{
					if (UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings())
					{
						if (ControlData.InputAction)
						{
							TArray<FKey> BoundKeys = Subsystem->QueryKeysMappedToAction(ControlData.InputAction);
							if (BoundKeys.Num() > 0)
							{
								UE_LOG(LogTemp, Display, TEXT("ControlData.InputAction = %s"), *ControlData.InputAction->GetName());
								UControlBase* ControlBase = CreateWidget<UControlBase>(World, ControlBaseClass);
								
								ControlBase->SetControlText(*DescriptionText);
								// Add Icons for the Keys of current input action
								for (FKey Key : BoundKeys)
								{
									UTexture2D** PointerKeyIcon = KeyboardIconKeymappingData->KeyIconMap.Find(Key);
									UTexture2D* KeyIcon;
									if (PointerKeyIcon)
									{
										KeyIcon = *PointerKeyIcon;
										UE_LOG(LogTemp, Display, TEXT("KeyIcon = %s"), *KeyIcon->GetName());
									}
									else
									{
										// Set a default blank key to the icon
										KeyIcon = BlankKeyIcon;
									}
									ControlBase->AddIconToUI(KeyIcon);
								}
								UE_LOG(LogTemp, Display, TEXT("Returning"));
								return ControlBase;
							}
						}
					}
				}
			}
		}
	}

	return nullptr;
}


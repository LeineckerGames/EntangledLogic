#include "SettingsTabControls.h"
#include "Components/InputKeySelector.h"
#include "Components/ScrollBox.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"

// NativeConstruct is called when the widget is constructed and added to the viewport
void USettingsTabControls::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind the button changed events to their respective functions
    if (RotateLeftKeySelector) RotateLeftKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnRotateLeftKeySelected);
    if (RotateRightKeySelector) RotateRightKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnRotateRightKeySelected);
    if (ZoomInKeySelector) ZoomInKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnZoomInKeySelected);
    if (ZoomOutKeySelector) ZoomOutKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnZoomOutKeySelected);
    if (RotateGateKeySelector) RotateGateKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnRotateGateSelected);
    if (DeleteModeKeySelector) DeleteModeKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnDeleteModeSelected);
    if (EditModeKeySelector) EditModeKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnEditModeSelected);
    if (ForwardsKeySelector) ForwardsKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnForwardsKeySelected);
    if (BackwardsKeySelector) BackwardsKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnBackwardsKeySelected);
    if (LeftKeySelector) LeftKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnLeftKeySelected);
    if (RightKeySelector) RightKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnRightKeySelected);
    if (LeftClickKeySelector) LeftClickKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnLeftClickKeySelected);
    if (RightClickKeySelector) RightClickKeySelector->OnKeySelected.AddDynamic(this, &USettingsTabControls::OnRightClickKeySelected);

    // Get default key bindings / Get player saved key bindings
    /*
    FKey RotateLeftKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("CMLeft"));
    if (RotateLeftKey.IsValid()) RotateLeftKeySelector->SetSelectedKey(RotateLeftKey);
    FKey RotateRightKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("CMRight"));
    if (RotateRightKey.IsValid()) RotateRightKeySelector->SetSelectedKey(RotateRightKey);
    */
    FKey ZoomInKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("ZoomIn"));
    if (ZoomInKey.IsValid()) ZoomInKeySelector->SetSelectedKey(ZoomInKey);
    FKey ZoomOutKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("ZoomOut"));
    if (ZoomOutKey.IsValid()) ZoomOutKeySelector->SetSelectedKey(ZoomOutKey);
    FKey RotateGateKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("RotateFactory"));
    if (RotateGateKey.IsValid()) RotateGateKeySelector->SetSelectedKey(RotateGateKey);
    FKey EditingModeKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("EditingMode"));
    if (EditingModeKey.IsValid()) DeleteModeKeySelector->SetSelectedKey(EditingModeKey);
    FKey DeletionModeKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("DeletionMode"));
    if (DeletionModeKey.IsValid()) EditModeKeySelector->SetSelectedKey(DeletionModeKey);
    FKey MoveForwardKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("MoveForward"));
    if (MoveForwardKey.IsValid()) ForwardsKeySelector->SetSelectedKey(MoveForwardKey);
    FKey MoveBackwardKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("MoveBackward"));
    if (MoveBackwardKey.IsValid()) BackwardsKeySelector->SetSelectedKey(MoveBackwardKey);
    FKey MoveLeftKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("MoveLeft"));
    if (MoveLeftKey.IsValid()) LeftKeySelector->SetSelectedKey(MoveLeftKey);
    FKey MoveRightKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("MoveRight"));
    if (MoveRightKey.IsValid()) RightKeySelector->SetSelectedKey(MoveRightKey);
    FKey LMBKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("PlayerLeftClick"));
    if (LMBKey.IsValid()) LeftClickKeySelector->SetSelectedKey(LMBKey);
    FKey RMBKey = GetCurrentKeyForMapping(GetOwningPlayer(), FName("PlayerRightClick"));
    if (RMBKey.IsValid()) RightClickKeySelector->SetSelectedKey(RMBKey);
}

// Camera Rotate Left Rebind
void USettingsTabControls::OnRotateLeftKeySelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("CMLeft");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Camera Rotate Right Rebind
void USettingsTabControls::OnRotateRightKeySelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("CMLeft");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Camera Zoom In Right Rebind
void USettingsTabControls::OnZoomInKeySelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("ZoomIn");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Camera Zoom Out Rebind
void USettingsTabControls::OnZoomOutKeySelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("ZoomOut");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Gate Rotation Rebind
void USettingsTabControls::OnRotateGateSelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("RotateFactory");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Toggle Deletion Mode Rebind
void USettingsTabControls::OnDeleteModeSelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("DeletionMode");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Toggle Editing Mode Rebind
void USettingsTabControls::OnEditModeSelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("EditingMode");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Movement Forwards Key Rebind
void USettingsTabControls::OnForwardsKeySelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("MoveForward");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Movement Backwards Key Rebind
void USettingsTabControls::OnBackwardsKeySelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("MoveBackward");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Movement Left Key Rebind
void USettingsTabControls::OnLeftKeySelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("MoveLeft");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Movement Right Key Rebind
void USettingsTabControls::OnRightKeySelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("MoveRight");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Player Left Click Rebind
void USettingsTabControls::OnLeftClickKeySelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("PlayerLeftClick");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Player Right Click Rebind
void USettingsTabControls::OnRightClickKeySelected(FInputChord SelectedKey)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    // Get the Enhanced Input Local Player Subsystem
    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    // Get the User Settings object (Handles saving, loading, and tracking profiles)
    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return;

    // Create the mapping container object with the new key
    FName MappingName = FName("PlayerRightClick");
    FMapPlayerKeyArgs Args;
    Args.MappingName = MappingName;
    Args.NewKey = SelectedKey.Key;

    // Map the new key
    FGameplayTagContainer FailureReason;
    UserSettings->MapPlayerKey(Args, FailureReason);

    // Check if any errors were added to the failure container
    if (FailureReason.IsEmpty())
    {
        // Apply changes immediately to active mapping contexts
        UserSettings->ApplySettings();

        // Save asynchronously to local user configuration
        UserSettings->AsyncSaveSettings();
    }
    else
    {
        // Handle failure (e.g., log why mapping failed)
        UE_LOG(LogTemp, Warning, TEXT("Failed to map key. Reasons: %s"), *FailureReason.ToString());
    }
}

// Helper function to set values in the rebinds based on the current key mapping for a given action
FKey USettingsTabControls::GetCurrentKeyForMapping(APlayerController* PC, FName MappingName)
{
    if (!PC) return EKeys::Invalid;

    ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
    if (!LocalPlayer) return EKeys::Invalid;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return EKeys::Invalid;

    UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
    if (!UserSettings) return EKeys::Invalid;

    /*
    // Configure query options
    FPlayerMappableKeyQueryOptions QueryOptions;
    QueryOptions.MappingToFind = MappingName;

    // Get all mapped keys matching this MappingName
    TArray<FKey> MappedKeys = UserSettings->GetMappedKeysInProfile(QueryOptions);

    // Return the first valid bound key
    for (const FKey& Key : MappedKeys)
    {
        if (Key.IsValid())
        {
            return Key;
        }
    }
    */
    if (UEnhancedPlayerMappableKeyProfile* KeyProfile = UserSettings->GetActiveKeyProfile())
    {
        const TMap<FName, FKeyMappingRow>& Mappings = KeyProfile->GetPlayerMappingRows();

        if (const FKeyMappingRow* FoundRow = Mappings.Find(MappingName))
        {
            for (const FPlayerKeyMapping& PlayerMapping : FoundRow->Mappings)
            {
                FKey BoundKey = PlayerMapping.GetCurrentKey();
                if (BoundKey.IsValid())
                {
                    return BoundKey;
                }
            }
        }
    }

    return EKeys::Invalid;
}
// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EntangledLogic : ModuleRules
{
	public EntangledLogic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// for qpp, idk what it does. some ai shit might break and kill someone.
		bUseRTTI = true;

		// more ai slop
        bEnableExceptions = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags", "GeometryCollectionEngine",
															"UMG", "RHI", "AudioModulation", "QppPlugin", "MetasoundEngine", "Niagara" });

        //PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

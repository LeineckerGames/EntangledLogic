// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class QppPlugin : ModuleRules
{
	public QppPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        bUseRTTI = true;

        bEnableExceptions = true;

        // Qpp requires C++17 or higher (Unreal 5 usually defaults to C++20, but good to ensure)
        CppStandard = CppStandardVersion.Cpp20;

        // Get the path to your ThirdParty directory
        string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty"));

        // Add Eigen and Qpp to the include paths
        PublicIncludePaths.AddRange(
            new string[] {
                Path.Combine(ThirdPartyPath, "Eigen"),
                Path.Combine(ThirdPartyPath) // Adjust this to point to where qpp.h lives
            }
        );


        PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}

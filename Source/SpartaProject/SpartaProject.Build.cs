// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpartaProject : ModuleRules
{
	public SpartaProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"UMG",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

	}
}

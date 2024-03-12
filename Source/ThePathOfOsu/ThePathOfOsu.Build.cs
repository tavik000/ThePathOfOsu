// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ThePathOfOsu : ModuleRules
{
	public ThePathOfOsu(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}

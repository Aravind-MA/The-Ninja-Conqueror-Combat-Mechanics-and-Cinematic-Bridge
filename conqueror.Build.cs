// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class conqueror : ModuleRules
{
	public conqueror(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
	}
}

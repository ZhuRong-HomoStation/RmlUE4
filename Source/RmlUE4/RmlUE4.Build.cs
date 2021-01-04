// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RmlUE4 : ModuleRules
{
	public RmlUE4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UERmlUI", "RmlUI" });
	}
}

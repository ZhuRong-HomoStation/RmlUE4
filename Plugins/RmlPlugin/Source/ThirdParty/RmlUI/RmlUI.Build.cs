// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using Tools.DotNETCommon;
using UnrealBuildTool;

public class RmlUI : ModuleRules
{
	public RmlUI(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.CPlusPlus;
		
		// Dependencies 
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core" ,
			"freetype2" ,
		});
		
		// RML ui need RTTI
		bUseRTTI = true;
		
		// Disable IWYU 
		bEnforceIWYU = false;
		
		// Disable pch to fix header error 
		PCHUsage = PCHUsageMode.NoPCHs;

		// Add include path 
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
		
		// Add definition 
		PrivateDefinitions.AddRange(new string[]
		{
			"RmlCore_EXPORTS" ,
			"RmlDebugger_EXPORTS" ,
		});

		// Disable warning 
		UnsafeTypeCastWarningLevel = WarningLevel.Off;
		ShadowVariableWarningLevel = WarningLevel.Off;
		
		// Disable freetype(font) 
		// PrivateDefinitions.Add("RMLUI_NO_FONT_INTERFACE_DEFAULT");
	}
}

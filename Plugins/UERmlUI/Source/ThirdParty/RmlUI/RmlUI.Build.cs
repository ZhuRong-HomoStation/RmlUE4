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
			"UElibPNG" ,	// depended by freetype 
			"zlib" ,		// depended by freetype 
			"FreeType2" ,	// use free type lib 
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
			"_CRT_SECURE_NO_WARNINGS" ,	// Disable scanf warning 
		});

		// Disable warning 
		UnsafeTypeCastWarningLevel = WarningLevel.Off;
		ShadowVariableWarningLevel = WarningLevel.Off;
		
		// Disable freetype(font) 
		// PrivateDefinitions.Add("RMLUI_NO_FONT_INTERFACE_DEFAULT");
		
		// Enable lua 
		// PrivateDefinitions.Add("RMLUI_BUILD_LUA");
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using Tools.DotNETCommon;
using UnrealBuildTool;

public class RmlUI : ModuleRules
{
	public RmlUI(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		// Add include path 
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Include"));
		
		// Add binaries
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win/x64/lib/RmlCore.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win/x64/lib/RmlDebugger.lib"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/RmlCore.dll", 
				Path.Combine(ModuleDirectory, "Win/x64/bin/RmlCore.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/RmlDebugger.dll",
				Path.Combine(ModuleDirectory, "Win/x64/bin/RmlDebugger.dll"), StagedFileType.NonUFS);
			PublicDelayLoadDLLs.Add("RmlCore.dll");
			PublicDelayLoadDLLs.Add("RmlDebugger.dll");
		}
		else if (Target.Platform == UnrealTargetPlatform.Win32)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win/x86/lib/RmlCore.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win/x86/lib/RmlDebugger.lib"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/RmlCore.dll", 
				Path.Combine(ModuleDirectory, "Win/x86/bin/RmlCore.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/RmlDebugger.dll",
				Path.Combine(ModuleDirectory, "Win/x86/bin/RmlDebugger.dll"), StagedFileType.NonUFS);
			PublicDelayLoadDLLs.Add("RmlCore.dll");
			PublicDelayLoadDLLs.Add("RmlDebugger.dll");
		}
		else
		{
			Log.TraceError("Not support platform {0}", Target.Platform);
		}
	}
}

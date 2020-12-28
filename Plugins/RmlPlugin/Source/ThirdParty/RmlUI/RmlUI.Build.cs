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
			RuntimeDependencies.Add("$(BinaryOutputDir)/brotlicommon.dll",
				Path.Combine(ModuleDirectory, "Win/x64/bin/brotlicommon.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/brotlidec.dll",
				Path.Combine(ModuleDirectory, "Win/x64/bin/brotlidec.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/bz2.dll",
				Path.Combine(ModuleDirectory, "Win/x64/bin/bz2.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/freetype.dll",
				Path.Combine(ModuleDirectory, "Win/x64/bin/freetype.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/libpng16.dll",
				Path.Combine(ModuleDirectory, "Win/x64/bin/libpng16.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/zlib1.dll",
				Path.Combine(ModuleDirectory, "Win/x64/bin/zlib1.dll"), StagedFileType.NonUFS);
			PublicDelayLoadDLLs.Add("RmlCore.dll");
			PublicDelayLoadDLLs.Add("RmlDebugger.dll");
			PublicDelayLoadDLLs.Add("brotlicommon.dll");
			PublicDelayLoadDLLs.Add("brotlidec.dll");
			PublicDelayLoadDLLs.Add("bz2.dll");
			PublicDelayLoadDLLs.Add("freetype.dll");
			PublicDelayLoadDLLs.Add("libpng16.dll");
			PublicDelayLoadDLLs.Add("zlib1.dll");
		}
		else if (Target.Platform == UnrealTargetPlatform.Win32)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win/x86/lib/RmlCore.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win/x86/lib/RmlDebugger.lib"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/RmlCore.dll", 
				Path.Combine(ModuleDirectory, "Win/x86/bin/RmlCore.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/RmlDebugger.dll",
				Path.Combine(ModuleDirectory, "Win/x86/bin/RmlDebugger.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/brotlicommon.dll",
				Path.Combine(ModuleDirectory, "Win/x86/bin/brotlicommon.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/brotlidec.dll",
				Path.Combine(ModuleDirectory, "Win/x86/bin/brotlidec.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/bz2.dll",
				Path.Combine(ModuleDirectory, "Win/x86/bin/bz2.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/freetype.dll",
				Path.Combine(ModuleDirectory, "Win/x86/bin/freetype.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/libpng16.dll",
				Path.Combine(ModuleDirectory, "Win/x86/bin/libpng16.dll"), StagedFileType.NonUFS);
			RuntimeDependencies.Add("$(BinaryOutputDir)/zlib1.dll",
				Path.Combine(ModuleDirectory, "Win/x86/bin/zlib1.dll"), StagedFileType.NonUFS);
			PublicDelayLoadDLLs.Add("RmlCore.dll");
			PublicDelayLoadDLLs.Add("RmlDebugger.dll");
			PublicDelayLoadDLLs.Add("brotlicommon.dll");
			PublicDelayLoadDLLs.Add("brotlidec.dll");
			PublicDelayLoadDLLs.Add("bz2.dll");
			PublicDelayLoadDLLs.Add("freetype.dll");
			PublicDelayLoadDLLs.Add("libpng16.dll");
			PublicDelayLoadDLLs.Add("zlib1.dll");
		}
		else
		{
			Log.TraceError("Not support platform {0}", Target.Platform);
		}
	}
}

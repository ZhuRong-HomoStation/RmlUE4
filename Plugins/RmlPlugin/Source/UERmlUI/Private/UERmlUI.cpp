#include "UERmlUI.h"
#include "Logging.h"
#include "Interfaces/IPluginManager.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FUERmlUI"

void FUERmlUI::StartupModule()
{
	// register shader dictionary 
	FString ShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("RmlPlugin"))->GetBaseDir(),TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugin/RmlPlugin"), ShaderDir);
}

void FUERmlUI::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUERmlUI, UERmlUI);
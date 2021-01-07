#include "CoreMinimal.h"
#include "Logging.h"
#include "Interfaces/IPluginManager.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FUERmlUI"

class UERMLUI_API FUERmlUI : public IModuleInterface
{
public:
	void StartupModule() override
	{
		// register shader dictionary 
		FString ShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("UERmlUI"))->GetBaseDir(),TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/Plugin/UERmlUI"), ShaderDir);
	}
	void ShutdownModule() override
	{
		
	}
};

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUERmlUI, UERmlUI);
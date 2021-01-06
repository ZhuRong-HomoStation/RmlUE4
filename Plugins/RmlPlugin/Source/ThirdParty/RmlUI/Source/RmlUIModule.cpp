#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FRmlUI"

class RMLUI_API FRmlUI : public IModuleInterface
{
	public:
	void StartupModule() override { }
	void ShutdownModule() override { }
};

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRmlUI, RmlUI);
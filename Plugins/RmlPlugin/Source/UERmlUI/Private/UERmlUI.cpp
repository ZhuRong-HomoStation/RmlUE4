#include "UERmlUI.h"
#include "Logging.h"
#include "Modules/ModuleManager.h"
#include "RmlInterface/UERmlRenderInterface.h"
#include "RmlInterface/UERmlSystemInterface.h"
#include "RmlUi/Core.h"

#define LOCTEXT_NAMESPACE "FUERmlUI"

void FUERmlUI::StartupModule()
{
}

void FUERmlUI::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUERmlUI, UERmlUI);
#include "UERmlUI.h"
#include "Logging.h"
#include "Modules/ModuleManager.h"
#include "RmlInterface/UERmlFontInterface.h"
#include "RmlInterface/UERmlRenderInterface.h"
#include "RmlInterface/UERmlSystemInterface.h"
#include "RmlUi/Core.h"

#define LOCTEXT_NAMESPACE "FUERmlUI"

Rml::Context* RmlCtx = nullptr;
FUERmlSystemInterface	SystemInterface;
FUERmlRenderInterface	RenderInterface;
FUERmlFontInterface		FontInterface;

void FUERmlUI::StartupModule()
{
	// init interface
	Rml::SetSystemInterface(&SystemInterface);
	Rml::SetRenderInterface(&RenderInterface);
	Rml::SetFontEngineInterface(&FontInterface);

	// Initialize 
	Rml::Initialise();

	// create context 
	RmlCtx = Rml::CreateContext("Default", Rml::Vector2i(100, 100));
}

void FUERmlUI::ShutdownModule()
{
	// remove context 
	Rml::RemoveContext(RmlCtx->GetName());

	// shut down
	Rml::Shutdown();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUERmlUI, UERmlUI);
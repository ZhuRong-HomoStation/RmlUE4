#include "UERmlUI/Public/UERmlSubsystem.h"
#include "Render/TextureEntries.h"
#include "RmlUi/Core.h"

void UUERmlSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Rml::SetSystemInterface(&RmlSystemInterface);
	Rml::SetRenderInterface(&RmlRenderInterface);

	check(Rml::Initialise());
}

void UUERmlSubsystem::Deinitialize()
{
	Rml::Shutdown();
}

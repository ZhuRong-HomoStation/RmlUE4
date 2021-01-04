#include "UERmlUI/Public/UERmlSubsystem.h"
#include "Render/TextureEntries.h"
#include "RmlUi/Core.h"

void UUERmlSubsystem::SetImportTexture(FString Name, UTexture* InTexture)
{
	AllImportTextures.Add(Name, MakeShared<FTextureEntry, ESPMode::ThreadSafe>(InTexture));
}

UTexture* UUERmlSubsystem::GetImportTexture(FString Name)
{
	auto FoundTexture = AllImportTextures.Find(Name);
	return FoundTexture ? (*FoundTexture)->GetTexture() : nullptr;
}

void UUERmlSubsystem::RemoveImportTexture(FString Name)
{
	AllImportTextures.Remove(Name);
}

void UUERmlSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Rml::SetSystemInterface(&RmlSystemInterface);
	Rml::SetRenderInterface(&RmlRenderInterface);

	check(Rml::Initialise());
}

void UUERmlSubsystem::Deinitialize()
{
	Rml::Shutdown();
	AllImportTextures.Reset();
}

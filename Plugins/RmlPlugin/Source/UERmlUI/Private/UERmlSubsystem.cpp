#include "UERmlUI/Public/UERmlSubsystem.h"
#include "Render/TextureEntries.h"
#include "RmlUi/Core.h"

void UUERmlSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Rml::SetSystemInterface(&RmlSystemInterface);
	Rml::SetRenderInterface(&RmlRenderInterface);

	check(Rml::Initialise());

	// load font face
	FString FontPath = FPaths::ProjectContentDir() / TEXT("RmlAssets/assets/");
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-Bold.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-BoldItalic.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-Italic.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-Roman.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("NotoEmoji-Regular.ttf"))), true);
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("STKAITI.TTF"))));
}

void UUERmlSubsystem::Deinitialize()
{
	Rml::Shutdown();
}

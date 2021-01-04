#include "RmlUE4GameModeBase.h"

void ARmlUE4GameModeBase::BeginPlay()
{
	RmlWidget = SNew(SRmlWidget);
	FString DocPath = FPaths::ProjectContentDir() / TEXT("RmlAssets/assets/demo.rml");
	FString FontPath = FPaths::ProjectContentDir() / TEXT("RmlAssets/assets/");

	// load font face 
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-Bold.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-BoldItalic.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-Italic.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-Roman.otf"))));

	// load document  
	auto Doc = RmlWidget->GetContext()->LoadDocument(TCHAR_TO_UTF8(*DocPath));
	check(Doc);

	Doc->Show();
	RmlWidget->AddToViewport(GetWorld());
}

void ARmlUE4GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

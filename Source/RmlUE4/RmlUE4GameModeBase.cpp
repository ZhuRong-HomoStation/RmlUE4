#include "RmlUE4GameModeBase.h"
#include <sstream>
#include "RmlUi/Core/StreamMemory.h"
#include "Widgets/Images/SImage.h"


ARmlUE4GameModeBase::ARmlUE4GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARmlUE4GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// setup interface 
	Rml::SetSystemInterface(&RmlSystemInterface);
	Rml::SetRenderInterface(&RmlRenderInterface);
	
	// init rml environment 
	check(Rml::Initialise());

	// load font face
	FString FontPath = FPaths::ProjectContentDir() / TEXT("RmlAssets/assets/");
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-Bold.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-BoldItalic.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-Italic.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("Delicious-Roman.otf"))));
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("NotoEmoji-Regular.ttf"))), true);
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("STKAITI.TTF"))));
	
	// create context 
	Context = Rml::CreateContext("Test Context", Rml::Vector2i());

	// load document
	FString DocPath = FPaths::ProjectContentDir() / TEXT("RmlAssets/assets/demo.rml");
	Document = Context->LoadDocument(TCHAR_TO_UTF8(*DocPath));

	// show document 
	Document->Show();

	// create widget 
	auto RmlWidget = SNew(SRmlWidget)
	.RenderInterface(&RmlRenderInterface)
	.InitContext(Context);

	// add widget to viewport 
	RmlWidget->AddToViewport(GetWorld());
	
	// setup input mode 
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(RmlWidget);
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);

	// enable cursor 
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void ARmlUE4GameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARmlUE4GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// release context
	Rml::RemoveContext("Test Context");
	Context = nullptr;

	// shut down rml environment 
	Rml::Shutdown();
}

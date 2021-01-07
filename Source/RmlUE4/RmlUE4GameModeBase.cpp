#include "RmlUE4GameModeBase.h"
#include "Widgets/Images/SImage.h"
#include "Kismet/KismetSystemLibrary.h"
#include <sstream>

#include "RmlUi/Debugger/Debugger.h"


ARmlUE4GameModeBase::ARmlUE4GameModeBase()
	: Demo(nullptr)
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

	// setup context 
	Context->SetDensityIndependentPixelRatio(1.0f);

	// init debugger 
	// Rml::Debugger::Initialise(Context);

	// show debugger 
	// Rml::Debugger::SetVisible(true);
	
	// load document
	FString DocPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("RmlAssets/assets/Examples/demo.rml"));
	Demo = NewObject<URmlDemo>(this);
	Demo->Init(Context, DocPath);

	// show document 
	Demo->GetDocument()->Show();

	// create widget 
	auto RmlWidget = SNew(SRmlWidget)
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

	// shut down documents
	Demo->ShutDown();
	Demo = nullptr;
	
	// release context
	Rml::RemoveContext("Test Context");
	Context = nullptr;

	// shut down rml environment 
	Rml::Shutdown();
}

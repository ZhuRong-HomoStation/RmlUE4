#include "RmlUE4GameModeBase.h"
#include "Widgets/Images/SImage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RmlUi/Debugger/Debugger.h"
#include <sstream>


ARmlUE4GameModeBase::ARmlUE4GameModeBase()
	: MainDemo(nullptr)
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
	Rml::LoadFontFace(TCHAR_TO_UTF8(*(FontPath + TEXT("STKAITI.TTF"))), true);
	
	// create context 
	Context = Rml::CreateContext("Test Context", Rml::Vector2i());

	// setup context 
	Context->SetDensityIndependentPixelRatio(1.0f);

	// init debugger 
	// Rml::Debugger::Initialise(Context);

	// show debugger 
	// Rml::Debugger::SetVisible(true);
	
	// load demo selector 
	FString BasePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("RmlAssets/assets/Examples/"));
	DemoSelector = NewObject<URmlDocument>(this);
	DemoSelector->Init(Context, BasePath + TEXT("selectbar.rml"));
	DemoSelector->Show();

	// load demos
	_LoadDemos(BasePath);
	
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
	DemoSelector->ShutDown();
	MainDemo->ShutDown();
	BenchMark->ShutDown();
	
	// release context
	Rml::RemoveContext("Test Context");
	Context = nullptr;

	// shut down rml environment 
	Rml::Shutdown();
}

void ARmlUE4GameModeBase::_LoadDemos(const FString& InBasePath)
{
	// setup notify object 
	DemoSelector->SetNotifyObject(TEXT("Controller"), this);

	// main demo 
	MainDemo = NewObject<URmlDemo>(this);
	MainDemo->Init(Context, InBasePath + TEXT("demo.rml"));
	
	// benchmark
	BenchMark = NewObject<URmlBenchmark>(this);
	BenchMark->Init(Context, InBasePath + TEXT("benchmark.rml"));
}

void ARmlUE4GameModeBase::_ChangeShowItem(URmlDocument* InDocument)
{
	if (CurrentElement == BenchMark)
	{
		BenchMark->bDoPerformanceTest = false;
	}
	
	if (CurrentElement) CurrentElement->GetDocument()->Hide();
	InDocument->GetDocument()->Show();
	CurrentElement = InDocument;

	if (InDocument == BenchMark)
	{
		BenchMark->bDoPerformanceTest = true;
	}
}

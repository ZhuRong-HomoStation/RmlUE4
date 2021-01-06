#include "RmlUE4GameModeBase.h"
#include <sstream>
#include "UERmlSubsystem.h"
#include "RmlUi/Core/StreamMemory.h"
#include "Widgets/Images/SImage.h"

Rml::ElementDocument* Doc;

ARmlUE4GameModeBase::ARmlUE4GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARmlUE4GameModeBase::BeginPlay()
{
	RmlWidget = SNew(SRmlWidget);
	FString DocPath = FPaths::ProjectContentDir() / TEXT("RmlAssets/assets/transform.rml");
	
	// load document  
	Doc = RmlWidget->GetContext()->LoadDocument(TCHAR_TO_UTF8(*DocPath));
	check(Doc);
	// Doc->GetElementById("title")->SetInnerRML("Demo sample");
	Doc->SetProperty(Rml::PropertyId::Left, Rml::Property(400, Rml::Property::PX));
	Doc->SetProperty(Rml::PropertyId::Top, Rml::Property(140, Rml::Property::PX));

	Doc->Show();
	RmlWidget->AddToViewport(GetWorld());
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(RmlWidget);
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void ARmlUE4GameModeBase::Tick(float DeltaSeconds)
{
	static float Degree = 0;
	Degree += DeltaSeconds * 50;
	if (Doc)
	{
		std::stringstream s;
		s << "perspective(" << 800.f << "px) ";
		s << "rotate3d(0.0, 1.0, 0.0, " << Degree << "deg)";
		Doc->SetProperty("transform", s.str().c_str());
	}
}

void ARmlUE4GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

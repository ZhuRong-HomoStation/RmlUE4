#include "RmlUE4GameModeBase.h"

#include "UERmlSubsystem.h"
#include "Widgets/Images/SImage.h"

void ARmlUE4GameModeBase::BeginPlay()
{
	RmlWidget = SNew(SRmlWidget);
	FString DocPath = FPaths::ProjectContentDir() / TEXT("RmlAssets/assets/demo.rml");
	
	// load document  
	auto Doc = RmlWidget->GetContext()->LoadDocument(TCHAR_TO_UTF8(*DocPath));
	check(Doc);
	Doc->GetElementById("title")->SetInnerRML("Demo sample");
	Doc->SetProperty(Rml::PropertyId::Left, Rml::Property(150, Rml::Property::PX));
	Doc->SetProperty(Rml::PropertyId::Top, Rml::Property(50, Rml::Property::PX));

	Doc->Show();
	RmlWidget->AddToViewport(GetWorld());
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(RmlWidget);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void ARmlUE4GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

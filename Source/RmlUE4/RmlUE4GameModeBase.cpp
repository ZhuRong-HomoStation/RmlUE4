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

	// debug font
	// FTimerHandle Handle;
	// GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([this]
	// {
	// 	static FSlateBrush Brush;
	// 	Brush.SetResourceObject(UUERmlSubsystem::Get()->GetRmlRenderInterface().GetTexture()->BoundTexture);
	// 	auto FontImg = SNew(SImage).Image(&Brush);
	// 	GetWorld()->GetGameViewport()->AddViewportWidgetContent(FontImg, 11);
	// }), 2, false);
}

void ARmlUE4GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

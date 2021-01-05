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

	Doc->Show();
	RmlWidget->AddToViewport(GetWorld());

	// debug font
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([this]
	{
		if (UUERmlSubsystem::Get()->GetRmlRenderInterface().GetCreatedTextures().Num() == 0) return;
		static FSlateBrush Brush;
		Brush.SetResourceObject(UUERmlSubsystem::Get()->GetRmlRenderInterface().GetCreatedTextures()[0]->BoundTexture);
		auto FontImg = SNew(SImage).Image(&Brush);
		GetWorld()->GetGameViewport()->AddViewportWidgetContent(FontImg, 11);
	}), 2, false);
}

void ARmlUE4GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

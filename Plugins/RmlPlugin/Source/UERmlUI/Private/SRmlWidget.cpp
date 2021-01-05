#include "SRmlWidget.h"
#include "RmlHelper.h"
#include "UERmlSubsystem.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"

void SRmlWidget::Construct(const FArguments& InArgs)
{
	BoundContext = Rml::CreateContext(TCHAR_TO_UTF8(*InArgs._Name), Rml::Vector2i(0));
}

SRmlWidget::~SRmlWidget()
{
	for (auto Doc : Documents)
	{
		Doc->Close();
		BoundContext->UnloadDocument(Doc);
	}
	Documents.Reset();
	Rml::RemoveContext(BoundContext->GetName());
}

Rml::ElementDocument* SRmlWidget::LoadDocument(const FString& InPath)
{
	auto Doc = BoundContext->LoadDocument(TCHAR_TO_UTF8(*InPath));
	if (Doc) Documents.Add(Doc);
	return Doc;
}

void SRmlWidget::DestroyDocument(Rml::ElementDocument* InDoc)
{
	auto Index = Documents.IndexOfByKey(InDoc);
	if (Index == INDEX_NONE) return;
	InDoc->Close();
	Documents.RemoveAtSwap(Index);
}

bool SRmlWidget::AddToViewport(UWorld* InWorld, int32 ZOrder)
{
	// get game view port 
	UGameViewportClient* ViewportClient = InWorld->GetGameViewport();
	if (!ViewportClient) return false;

	// add to view port 
	ViewportClient->AddViewportWidgetContent(this->AsShared(), ZOrder + 10);

	return true;
}

void SRmlWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{	
	// update size 
	FVector2D CurSizeUE = AllottedGeometry.GetLocalSize();
	Rml::Vector2i CurSize((int)CurSizeUE.X, (int)CurSizeUE.Y);
	Rml::Vector2i LastSize = BoundContext->GetDimensions();
	if (CurSize != LastSize)
	{
		BoundContext->SetDimensions(CurSize);
	}

	// call update 
	BoundContext->Update();
}

int32 SRmlWidget::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	auto& RenderInterface = UUERmlSubsystem::Get()->GetRmlRenderInterface();

	RenderInterface.CurrentElementList = &OutDrawElements;
	RenderInterface.CurrentLayer = LayerId;

	// local space -> render space 
	RenderInterface.RmlWidgetRenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();
	RenderInterface.RmlToWidgetMatrix = RenderInterface.RmlWidgetRenderTransform.To3DMatrix();
	
	// render space -> NDC space 
	FVector2D SubPart, AddPart;
	SubPart.X = MyCullingRect.Right - MyCullingRect.Left;
	SubPart.Y = MyCullingRect.Bottom - MyCullingRect.Top;
	AddPart.X = MyCullingRect.Right + MyCullingRect.Left;
	AddPart.Y = MyCullingRect.Bottom + MyCullingRect.Top;
	RenderInterface.OrthoMatrix = FMatrix(
			FPlane(2.0f / SubPart.X,0.0f,			0.0f,		0.0f),
			FPlane(0.0f,			2.0f / SubPart.Y,	0.0f,		0.0f),
			FPlane(0.0f,			0.0f,			1.0f,		0.0f),
			FPlane(-AddPart.X / SubPart.X,		-AddPart.Y / SubPart.Y,			0,			1.0f));
	RenderInterface.RmlRenderMatrix = RenderInterface.RmlToWidgetMatrix * RenderInterface.OrthoMatrix;
	RenderInterface.ViewportRect = MyCullingRect;
	
	BoundContext->Render();
	
	return LayerId + 1;
}

FVector2D SRmlWidget::ComputeDesiredSize(float) const
{
	return FVector2D::ZeroVector;
}

FReply SRmlWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	auto ModifierState = InKeyEvent.GetModifierKeys();
	return BoundContext->ProcessKeyDown(
		FRmlHelper::ConvertKey(InKeyEvent.GetKey()),
		FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	auto ModifierState = InKeyEvent.GetModifierKeys();
	return BoundContext->ProcessKeyUp(
        FRmlHelper::ConvertKey(InKeyEvent.GetKey()),
        FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& InCharacterEvent)
{
	return BoundContext->ProcessTextInput(InCharacterEvent.GetCharacter()) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto ModifierState = MouseEvent.GetModifierKeys();
	auto MousePos = MouseEvent.GetScreenSpacePosition();

	// screen space -> local space 
	MousePos = MyGeometry.GetAccumulatedRenderTransform().Inverse().TransformPoint(MousePos);
	
	return BoundContext->ProcessMouseMove(
		MousePos.X,
		MousePos.Y,
		FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto ModifierState = MouseEvent.GetModifierKeys();
	return BoundContext->ProcessMouseButtonDown(
		FRmlHelper::GetMouseKey(MouseEvent.GetEffectingButton()),
		FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto ModifierState = MouseEvent.GetModifierKeys();
	return BoundContext->ProcessMouseButtonUp(
        FRmlHelper::GetMouseKey(MouseEvent.GetEffectingButton()),
        FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto ModifierState = MouseEvent.GetModifierKeys();
	return BoundContext->ProcessMouseWheel(
        -MouseEvent.GetWheelDelta(),
        FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

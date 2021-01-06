﻿#include "SRmlWidget.h"
#include "RmlHelper.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "RmlUi/Core.h"

void SRmlWidget::Construct(const FArguments& InArgs)
{
	RenderInterface = InArgs._RenderInterface;
	BoundContext = InArgs._InitContext;
	bEnableRml = InArgs._InitEnableRml;
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

bool SRmlWidget::RemoveFromParent(UWorld* InWorld)
{
	// get game view port 
	UGameViewportClient* ViewportClient = InWorld->GetGameViewport();
	if (!ViewportClient) return false;

	// remove from viewport 
	ViewportClient->RemoveViewportWidgetContent(this->AsShared());

	return true;
}

void SRmlWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (!bEnableRml || !BoundContext) return;
	
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
	if (!bEnableRml || !BoundContext || !RenderInterface) return LayerId;
	
	// set global info for call draw api 
	RenderInterface->CurrentElementList = &OutDrawElements;
	RenderInterface->CurrentLayer = LayerId;

	// set render transform for translate clip rect 
	RenderInterface->RmlWidgetRenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();
	
	// rml local space -> slate render space 
	RenderInterface->RmlRenderMatrix = RenderInterface->RmlWidgetRenderTransform.To3DMatrix();

	// slate render space -> NDC(Normalized Device Space)
	FVector2D Size = OutDrawElements.GetPaintWindow()->GetSizeInScreen();;
	RenderInterface->RmlRenderMatrix *= FMatrix(
			FPlane(2.0f / Size.X,0.0f,			0.0f,		0.0f),
			FPlane(0.0f,			-2.0f / Size.Y,	0.0f,		0.0f),
			FPlane(0.0f,			0.0f,			1.f / 5000.f,0.0f),
			FPlane(-1,			1,				0.5f,		1.0f));

	// set viewport rect for compute clip rect 
	RenderInterface->ViewportRect = MyCullingRect;

	// call render api 
	BoundContext->Render();

	// next layer 
	return LayerId + 1;
}

FReply SRmlWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (!bEnableRml || !BoundContext) return FReply::Unhandled();

	// reroute key event 
	auto ModifierState = InKeyEvent.GetModifierKeys();
	return BoundContext->ProcessKeyDown(
		FRmlHelper::ConvertKey(InKeyEvent.GetKey()),
		FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Unhandled() : FReply::Handled();
}

FReply SRmlWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (!bEnableRml || !BoundContext) return FReply::Unhandled();

	// reroute key event 
	auto ModifierState = InKeyEvent.GetModifierKeys();
	return BoundContext->ProcessKeyUp(
        FRmlHelper::ConvertKey(InKeyEvent.GetKey()),
        FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Unhandled() : FReply::Handled();
}

FReply SRmlWidget::OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& InCharacterEvent)
{
	if (!bEnableRml || !BoundContext) return FReply::Unhandled();

	// reroute ANIS input 
	return BoundContext->ProcessTextInput(InCharacterEvent.GetCharacter()) ? FReply::Unhandled() : FReply::Handled();
}

FReply SRmlWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bEnableRml || !BoundContext) return FReply::Unhandled();

	// get screen pos 
	auto MousePos = MouseEvent.GetScreenSpacePosition();

	// screen space -> rml local space 
	MousePos = MyGeometry.GetAccumulatedRenderTransform().Inverse().TransformPoint(MousePos);

	// reroute ANIS input 
	auto ModifierState = MouseEvent.GetModifierKeys();
	return BoundContext->ProcessMouseMove(
		MousePos.X,
		MousePos.Y,
		FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Unhandled() : FReply::Handled();
}

FReply SRmlWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bEnableRml || !BoundContext) return FReply::Unhandled();

	// reroute mouse input 
	auto ModifierState = MouseEvent.GetModifierKeys();
	return BoundContext->ProcessMouseButtonDown(
		FRmlHelper::GetMouseKey(MouseEvent.GetEffectingButton()),
		FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Unhandled() : FReply::Handled();
}

FReply SRmlWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bEnableRml || !BoundContext) return FReply::Unhandled();

	// reroute mouse input 
	auto ModifierState = MouseEvent.GetModifierKeys();
	return BoundContext->ProcessMouseButtonUp(
        FRmlHelper::GetMouseKey(MouseEvent.GetEffectingButton()),
        FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Unhandled() : FReply::Handled();
}

FReply SRmlWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bEnableRml || !BoundContext) return FReply::Unhandled();

	// reroute mouse input 
	auto ModifierState = MouseEvent.GetModifierKeys();
	return BoundContext->ProcessMouseWheel(
        -MouseEvent.GetWheelDelta(),
        FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Unhandled() : FReply::Handled();
}

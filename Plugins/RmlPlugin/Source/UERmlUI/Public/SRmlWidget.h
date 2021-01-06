#pragma once
#include "RmlInterface/UERmlRenderInterface.h"
#include "RmlUi/Core.h"
#include "Widgets/SLeafWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UERMLUI_API SRmlWidget : public SLeafWidget
{
	SLATE_BEGIN_ARGS(SRmlWidget)
		: _RenderInterface(nullptr)
		, _InitContext(nullptr)
		, _InitEnableRml(true)
	{}
		SLATE_ARGUMENT(FUERmlRenderInterface*, RenderInterface)
		SLATE_ARGUMENT(Rml::Context*, InitContext)
		SLATE_ARGUMENT(bool, InitEnableRml)
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);

	// widget functional 
	bool AddToViewport(UWorld* InWorld, int32 ZOrder = 0);
	bool RemoveFromParent(UWorld* InWorld);
	
	void Context(Rml::Context* InContext) { BoundContext = InContext; }
	Rml::Context* Context() const { return BoundContext; }
protected:
	// ~Begin SWidget API
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float) const override { return FVector2D::ZeroVector; }
	
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	virtual FReply OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& InCharacterEvent) override;

	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual bool SupportsKeyboardFocus() const override { return true; }
	// ~End SWidget API
private:
	bool					bEnableRml;
	Rml::Context*			BoundContext;
	FUERmlRenderInterface*	RenderInterface;
};

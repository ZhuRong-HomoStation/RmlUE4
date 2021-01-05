#pragma once

class FRmlMesh;

class FRmlDrawer : public ICustomSlateElement
{
public:
	// ~Begin ICustomSlateElement API 
	virtual void DrawRenderThread(FRHICommandListImmediate& RHICmdList, const void* RenderTarget) override;
	// ~End ICustomSlateElement API 

	bool IsValid() { return BoundMesh.IsValid(); }
	void Reset() { BoundMesh.Reset(); }
public:
	TSharedPtr<FRmlMesh, ESPMode::ThreadSafe>	BoundMesh;
	FMatrix										RenderTransform;
	bool					bEnableScissorRect = false;
	FIntRect				ScissorRect;
};

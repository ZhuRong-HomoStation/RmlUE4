#pragma once

class FRmlMesh;

class FRmlDrawer : public ICustomSlateElement , public FGCObject
{
public:
	// ~Begin ICustomSlateElement API 
	virtual void DrawRenderThread(FRHICommandListImmediate& RHICmdList, const void* RenderTarget) override;
	// ~End ICustomSlateElement API 
	
	// ~Begin FGCObject API 
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// ~End FGCObject API

	bool IsValid() { return BoundTexture && BoundMesh; }
	void Reset() { BoundMesh.Reset(); BoundTexture = nullptr; }
public:
	TSharedPtr<FRmlMesh>		BoundMesh;
	UTexture2D*					BoundTexture;
	FMatrix						RenderTransform;
};

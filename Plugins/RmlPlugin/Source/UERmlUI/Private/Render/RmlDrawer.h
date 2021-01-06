#pragma once

class FRmlMesh;

struct FRmlMeshDrawInfo
{
	FRmlMeshDrawInfo();
	FRmlMeshDrawInfo(
        TSharedPtr<FRmlMesh, ESPMode::ThreadSafe> InBoundMesh ,
        const FMatrix& InRenderTransform ,
        const FIntRect& InScissorRect)
	        : BoundMesh(InBoundMesh)
			, RenderTransform(InRenderTransform)
			, ScissorRect(InScissorRect)
		{}
	
	TSharedPtr<FRmlMesh, ESPMode::ThreadSafe>	BoundMesh;
	FMatrix										RenderTransform;
	FIntRect									ScissorRect;
};

class FRmlDrawer : public ICustomSlateElement
{
public:
	FRmlDrawer(bool bUsing = false);
	
	// ~Begin ICustomSlateElement API 
	virtual void DrawRenderThread(FRHICommandListImmediate& RHICmdList, const void* RenderTarget) override;
	// ~End ICustomSlateElement API 

	FORCEINLINE void EmplaceMesh(
		TSharedPtr<FRmlMesh, ESPMode::ThreadSafe> InBoundMesh ,
		const FMatrix& InRenderTransform ,
		const FIntRect& InScissorRect)
	{
		Info.Emplace(InBoundMesh, InRenderTransform, InScissorRect);
	}
	
	bool IsFree() const { return bIsFree; }
	void MarkUsing() { bIsFree = false; }
	void MarkFree() { bIsFree = true; }
private:
	TArray<FRmlMeshDrawInfo>		Info;
	bool							bIsFree;
};

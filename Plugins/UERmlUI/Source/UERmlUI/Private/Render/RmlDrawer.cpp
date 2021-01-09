#include "RmlDrawer.h"
#include "ClearQuad.h"
#include "Logging.h"
#include "RmlMesh.h"
#include "RmlShader.h"
#include "Render/TextureEntries.h"

FRmlDrawer::FRmlDrawer(bool bUsing)
	: bIsFree(!bUsing)
{
}

void FRmlDrawer::DrawRenderThread(FRHICommandListImmediate& RHICmdList, const void* RenderTarget)
{
	// check thread 
	check(IsInRenderingThread() || IsInParallelRenderingThread());
	
	// early out
	if (DrawList.Num() == 0)
	{
		MarkFree();
		return;
	}

	// get render target 
	FTexture2DRHIRef* RT = (FTexture2DRHIRef*)RenderTarget;

	// begin pass 
	FRHIRenderPassInfo RPInfo(*RT, ERenderTargetActions::Load_Store);
	RHICmdList.BeginRenderPass(RPInfo, TEXT("DrawRmlMesh"));
	
	// Get shader
	auto ShaderMap = GetGlobalShaderMap(ERHIFeatureLevel::SM5);
 	TShaderMapRef<FRmlShaderVs> Vs(ShaderMap);
	TShaderMapRef<FRmlShaderPs> Ps(ShaderMap);
	TShaderMapRef<FRmlShaderPsNoTex> PsNoTex(ShaderMap);
	
	// Set PSO info 
	FGraphicsPipelineStateInitializer PSOInitializer;
	RHICmdList.ApplyCachedRenderTargets(PSOInitializer);
	PSOInitializer.BoundShaderState.VertexDeclarationRHI = FRmlMesh::GetMeshDeclaration();
	PSOInitializer.BoundShaderState.VertexShaderRHI = Vs.GetVertexShader();
	PSOInitializer.BoundShaderState.PixelShaderRHI = Ps.GetPixelShader();
	PSOInitializer.PrimitiveType = PT_TriangleList;
	PSOInitializer.BlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_SourceAlpha, BF_InverseSourceAlpha>::GetRHI();
	PSOInitializer.RasterizerState = TStaticRasterizerState<>::GetRHI();
	PSOInitializer.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
	FGraphicsPipelineStateInitializer PSOInitializerNoTex;
	RHICmdList.ApplyCachedRenderTargets(PSOInitializerNoTex);
	PSOInitializerNoTex.BoundShaderState.VertexDeclarationRHI = FRmlMesh::GetMeshDeclaration();
	PSOInitializerNoTex.BoundShaderState.VertexShaderRHI = Vs.GetVertexShader();
	PSOInitializerNoTex.BoundShaderState.PixelShaderRHI = PsNoTex.GetPixelShader();
	PSOInitializerNoTex.PrimitiveType = PT_TriangleList;
	PSOInitializerNoTex.BlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_SourceAlpha, BF_InverseSourceAlpha>::GetRHI();
	PSOInitializerNoTex.RasterizerState = TStaticRasterizerState<>::GetRHI();
	PSOInitializerNoTex.DepthStencilState = TStaticDepthStencilState<false, CF_Always, false, CF_Always>::GetRHI();
	
	// Init pso
	TSharedPtr<FRmlTextureEntry, ESPMode::ThreadSafe> CurTexture = DrawList[0].BoundMesh->BoundTexture;
	{
		SetGraphicsPipelineState(
	                    RHICmdList,
	                   CurTexture.IsValid() ? PSOInitializer : PSOInitializerNoTex);
		if (CurTexture.IsValid()) Ps->SetParameters(RHICmdList, Ps.GetPixelShader(), CurTexture->GetTextureRHI());
	}
	
	// Draw elements
	for (auto& DrawInfo : DrawList)
	{
		// Get new texture
		auto NewTexture = DrawInfo.BoundMesh->BoundTexture;
		
		// Change texture 
		if (NewTexture != CurTexture)
		{
			// Change PSO 
			if (CurTexture.IsValid() != NewTexture.IsValid())
			{
				SetGraphicsPipelineState(
					RHICmdList,
					NewTexture.IsValid() ? PSOInitializer : PSOInitializerNoTex);
			}

			// Change texture
			if (NewTexture.IsValid()) Ps->SetParameters(RHICmdList, Ps.GetPixelShader(), NewTexture->GetTextureRHI());

			// Update current texture
			CurTexture = NewTexture;
		}

		// Set transform 
		Vs->SetParameters(RHICmdList, DrawInfo.RenderTransform);

		// Set scissor rect
		RHICmdList.SetScissorRect(
			true,
			DrawInfo.ScissorRect.Min.X,
			DrawInfo.ScissorRect.Min.Y,
			DrawInfo.ScissorRect.Max.X,
			DrawInfo.ScissorRect.Max.Y);

		// Render mesh
		DrawInfo.BoundMesh->DrawMesh(RHICmdList);
	}
	
	// Reset draw info
	DrawList.Reset();
	
	// Mark free
	MarkFree();
	
	RHICmdList.EndRenderPass();
}

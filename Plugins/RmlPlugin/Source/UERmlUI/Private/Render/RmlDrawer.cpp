#include "RmlDrawer.h"
#include "Logging.h"
#include "RmlMesh.h"
#include "RmlShader.h"
#include "Render/TextureEntries.h"

void FRmlDrawer::DrawRenderThread(FRHICommandListImmediate& RHICmdList, const void* RenderTarget)
{
	// Get shader  
 	TShaderMapRef<FRmlShaderVs> Vs(GetGlobalShaderMap(ERHIFeatureLevel::SM5));
	TShaderMapRef<FRmlShaderPs> Ps(GetGlobalShaderMap(ERHIFeatureLevel::SM5));
	TShaderMapRef<FRmlShaderPsNoTex> PsNoTex(GetGlobalShaderMap(ERHIFeatureLevel::SM5));
	
	// Set PSO info 
	FGraphicsPipelineStateInitializer PSOInitializer;
	RHICmdList.ApplyCachedRenderTargets(PSOInitializer);
	PSOInitializer.BoundShaderState.VertexDeclarationRHI = FRmlMesh::GetMeshDeclaration();
	PSOInitializer.BoundShaderState.VertexShaderRHI = Vs.GetVertexShader();
	PSOInitializer.BoundShaderState.PixelShaderRHI = BoundMesh->BoundTexture ? Ps.GetPixelShader() : PsNoTex.GetPixelShader();
	PSOInitializer.PrimitiveType = PT_TriangleList;
	PSOInitializer.BlendState = TStaticBlendState<CW_RGBA, BO_Add, BF_SourceAlpha, BF_InverseSourceAlpha>::GetRHI();
	PSOInitializer.RasterizerState = TStaticRasterizerState<>::GetRHI();
	PSOInitializer.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();

	// Get PSO 
	SetGraphicsPipelineState(RHICmdList, PSOInitializer);

	// Set Params
	Vs->SetParameters(RHICmdList, RenderTransform);
	if (BoundMesh->BoundTexture)
	{
		Ps->SetParameters(RHICmdList, Ps.GetPixelShader(), BoundMesh->BoundTexture->GetTextureRHI());
	}

	// Draw 
	RHICmdList.SetScissorRect(true, ScissorRect.Min.X, ScissorRect.Min.Y, ScissorRect.Max.X, ScissorRect.Max.Y);
	BoundMesh->DrawMesh(RHICmdList);

	// Reset self 
	Reset();
}

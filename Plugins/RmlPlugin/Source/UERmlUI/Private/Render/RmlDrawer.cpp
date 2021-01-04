#include "RmlDrawer.h"


#include "RmlMesh.h"
#include "RmlShader.h"
#include "TextureEntries.h"

void FRmlDrawer::DrawRenderThread(FRHICommandListImmediate& RHICmdList, const void* RenderTarget)
{
	// Get shader  
 	TShaderMapRef<FRmlShaderVs> Vs(GetGlobalShaderMap(ERHIFeatureLevel::SM5));
	TShaderMapRef<FRmlShaderPs> Ps(GetGlobalShaderMap(ERHIFeatureLevel::SM5));
	TShaderMapRef<FRmlShaderPsNoTex> PsNoTex(GetGlobalShaderMap(ERHIFeatureLevel::SM5));
	
	// Set PSO 
	FGraphicsPipelineStateInitializer PSOInitializer;
	RHICmdList.ApplyCachedRenderTargets(PSOInitializer);
	PSOInitializer.BoundShaderState.VertexDeclarationRHI = FRmlMesh::GetMeshDeclaration();
	PSOInitializer.BoundShaderState.VertexShaderRHI = Vs.GetVertexShader();
	PSOInitializer.BoundShaderState.PixelShaderRHI = BoundMesh->BoundTexture ? Ps.GetPixelShader() : PsNoTex.GetPixelShader();
	PSOInitializer.PrimitiveType = PT_TriangleList;
	PSOInitializer.BlendState = TStaticBlendState<>::GetRHI();
	PSOInitializer.RasterizerState = TStaticRasterizerState<>::GetRHI();
	PSOInitializer.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
	
	SetGraphicsPipelineState(RHICmdList, PSOInitializer);

	// Set Params
	Vs->SetParameters(RHICmdList, RenderTransform);
	if (BoundMesh->BoundTexture)
	{
		Ps->SetParameters(RHICmdList, Ps.GetPixelShader(), BoundMesh->BoundTexture->GetTextureRHI());
	}

	// Draw 
	BoundMesh->DrawMesh(RHICmdList);

	// Reset self 
	Reset();
}

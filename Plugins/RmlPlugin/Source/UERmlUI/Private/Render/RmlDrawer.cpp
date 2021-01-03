﻿#include "RmlDrawer.h"


#include "RmlMesh.h"
#include "RmlShader.h"

void FRmlDrawer::DrawRenderThread(FRHICommandListImmediate& RHICmdList, const void* RenderTarget)
{
	// Set PSO 
	TShaderMapRef<FRmlShaderVs> Vs(GetGlobalShaderMap(ERHIFeatureLevel::SM5));
	TShaderMapRef<FRmlShaderPs> Ps(GetGlobalShaderMap(ERHIFeatureLevel::SM5));
	FGraphicsPipelineStateInitializer PSOInitializer;
	PSOInitializer.BoundShaderState.VertexDeclarationRHI = GSimpleElementVertexDeclaration.VertexDeclarationRHI;
	PSOInitializer.BoundShaderState.VertexShaderRHI = Vs.GetVertexShader();
	PSOInitializer.BoundShaderState.PixelShaderRHI = Ps.GetPixelShader();
	PSOInitializer.PrimitiveType = PT_TriangleList;
	PSOInitializer.BlendState = TStaticBlendState<>::GetRHI();
	SetGraphicsPipelineState(RHICmdList, PSOInitializer);

	// Set Params
	Vs->SetParameters(RHICmdList, RenderTransform);
	Ps->SetParameters(RHICmdList, Ps, BoundTexture->Resource->TextureRHI->GetTexture2D());

	// Draw 
	BoundMesh->DrawMesh(RHICmdList);

	// Reset self 
	Reset();
}

void FRmlDrawer::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(BoundTexture);
}

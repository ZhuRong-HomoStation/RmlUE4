#include "RmlMesh.h"
#include "RmlUi/Core/Vertex.h"

void FRmlMesh::Setup(
	Rml::Vertex* vertices,
	int num_vertices,
	int* indices,
	int num_indices,
	TSharedPtr<FRmlTextureEntry, ESPMode::ThreadSafe> InTexture)
{
	// copy vertices
	Vertices.SetNumUninitialized(num_vertices);
	for (int i = 0; i < num_vertices; ++i)
	{
		auto& SourceVertex = vertices[i];
		auto& VertexData = Vertices[i];
		VertexData.Color.X = SourceVertex.colour.red / 255.0;
		VertexData.Color.Y = SourceVertex.colour.green / 255.0;
		VertexData.Color.Z = SourceVertex.colour.blue / 255.0;
		VertexData.Color.W = SourceVertex.colour.alpha / 255.0;
		VertexData.Position.X = SourceVertex.position.x;
		VertexData.Position.Y = SourceVertex.position.y;
		VertexData.UV.X = SourceVertex.tex_coord.x;
		VertexData.UV.Y = SourceVertex.tex_coord.y;
	}

	// copy indices
	Indices.SetNumUninitialized(num_indices);
	for (int i = 0; i < num_indices; ++i)
	{
		Indices[i] = (uint16)indices[i];
	}

	// set up texture
	BoundTexture = InTexture;
}

void FRmlMesh::BuildMesh()
{
	if (Vertices.Num() == 0 || Indices.Num() == 0) return;
	check(Indices.Num() % 3 == 0);
	if (IsInRenderingThread())
	{
		BuildMesh_RenderThread();
	}
	else
	{
		ENQUEUE_RENDER_COMMAND(SetupOutputRemapMeshCmd)([this](FRHICommandListImmediate& RHICmdList)
        {
            BuildMesh_RenderThread();
        });
	}
	
}

void FRmlMesh::ReleaseMesh()
{
	VertexBufferRHI.SafeRelease();
	IndexBufferRHI.SafeRelease();
}

void FRmlMesh::DrawMesh(FRHICommandList& RHICmdList)
{
	check(IsInRenderingThread());

	RHICmdList.SetStreamSource(0, VertexBufferRHI, 0);
	RHICmdList.DrawIndexedPrimitive(IndexBufferRHI, 0, 0, NumVertices, 0, NumTriangles, 1);
}

FVertexDeclarationRHIRef FRmlMesh::GetMeshDeclaration()
{
 	static FVertexDeclarationRHIRef VertexDeclarationRHI;
	if (!VertexDeclarationRHI.IsValid())
	{
		FVertexDeclarationElementList Elements;
		uint16 Stride = sizeof(FVertexData);
		Elements.Add(FVertexElement(0,STRUCT_OFFSET(FVertexData,Position),VET_Float2,0,Stride));
		Elements.Add(FVertexElement(0,STRUCT_OFFSET(FVertexData,UV),VET_Float2,1,Stride));
		Elements.Add(FVertexElement(0,STRUCT_OFFSET(FVertexData,Color),VET_Float4,2,Stride));
		VertexDeclarationRHI = PipelineStateCache::GetOrCreateVertexDeclaration(Elements);
	}
	return VertexDeclarationRHI;
}

void FRmlMesh::BuildMesh_RenderThread()
{
	NumVertices = Vertices.Num();
	NumTriangles = Indices.Num() / 3;
	
	FRHIResourceCreateInfo VtxInfo;
	VtxInfo.ResourceArray = &Vertices;
	int32 VerticesBufferSize = sizeof(FVertexData) * Vertices.Num();
	VertexBufferRHI = RHICreateVertexBuffer(VerticesBufferSize, BUF_Static, VtxInfo);

	FRHIResourceCreateInfo IdxInfo;
	IdxInfo.ResourceArray = &Indices;
	int32 IndexBufferSize = sizeof(uint16) * Indices.Num();
	IndexBufferRHI = RHICreateIndexBuffer(sizeof(uint16), IndexBufferSize, BUF_Static, IdxInfo);
}

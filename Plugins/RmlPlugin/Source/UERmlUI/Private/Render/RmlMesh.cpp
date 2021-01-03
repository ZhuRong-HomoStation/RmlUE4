#include "RmlMesh.h"

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
	IndexBufferRHI = RHICreateIndexBuffer(IndexBufferSize, 0, BUF_Static, IdxInfo);
}

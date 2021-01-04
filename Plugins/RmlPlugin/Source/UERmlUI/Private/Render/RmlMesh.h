#pragma once

class FRmlTextureEntry;

class FRmlMesh : public TSharedFromThis<FRmlMesh, ESPMode::ThreadSafe>
{
public:
	struct FVertexData
	{
		FVector2D	Position;
		FVector2D	UV;
		FVector4	Color;
		FVertexData(const FVector2D& InPos, const FVector2D& InUV, const FVector4& InColor)
			: Position(InPos), UV(InUV), Color(InColor)
		{}
	};

	void BuildMesh();
	void ReleaseMesh();
	void DrawMesh(FRHICommandList& RHICmdList);

	static FVertexDeclarationRHIRef GetMeshDeclaration();
private:
	void BuildMesh_RenderThread();

public:
	TResourceArray<FVertexData>		Vertices;
	FVertexBufferRHIRef				VertexBufferRHI;

	TResourceArray<uint16>			Indices;
	FIndexBufferRHIRef				IndexBufferRHI;

	int32							NumVertices;
	int32							NumTriangles;

	TSharedPtr<FRmlTextureEntry, ESPMode::ThreadSafe>			BoundTexture;
};

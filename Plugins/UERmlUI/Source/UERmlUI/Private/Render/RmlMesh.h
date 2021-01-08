#pragma once

class FRmlTextureEntry;

namespace Rml
{
	struct Vertex;
}

class FRmlMesh : public TSharedFromThis<FRmlMesh, ESPMode::ThreadSafe>
{
public:
	struct FVertexData
	{
		FVector2D	Position;
		FColor		Color;
		FVector2D	UV;
		FVertexData(const FVector2D& InPos, const FVector2D& InUV, const FColor& InColor)
			: Position(InPos), Color(InColor), UV(InUV)
		{}
	};

	void Setup(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, TSharedPtr<FRmlTextureEntry, ESPMode::ThreadSafe> InTexture);
	void BuildMesh();
	void ReleaseMesh();
	void DrawMesh(FRHICommandList& RHICmdList);

	static FVertexDeclarationRHIRef GetMeshDeclaration();
public:
	TResourceArray<FVertexData>		Vertices;
	FVertexBufferRHIRef				VertexBufferRHI;

	TResourceArray<uint16>			Indices;
	FIndexBufferRHIRef				IndexBufferRHI;

	int32							NumVertices;
	int32							NumTriangles;

	TSharedPtr<FRmlTextureEntry, ESPMode::ThreadSafe>			BoundTexture;
};

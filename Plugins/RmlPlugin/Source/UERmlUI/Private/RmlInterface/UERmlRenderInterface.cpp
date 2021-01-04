#include "RmlInterface/UERmlRenderInterface.h"
#include "Rendering/DrawElements.h"
#include "Render/RmlDrawer.h"
#include "Render/RmlMesh.h"
#include "RmlUi/Core.h"

FUERmlRenderInterface::FUERmlRenderInterface()
{
}

Rml::CompiledGeometryHandle FUERmlRenderInterface::CompileGeometry(Rml::Vertex* vertices, int num_vertices,
	int* indices, int num_indices, Rml::TextureHandle texture)
{
	TSharedPtr<FRmlMesh, ESPMode::ThreadSafe> Mesh = (new FRmlMesh())->AsShared();

	// copy vertices
	Mesh->Vertices.SetNumUninitialized(num_vertices);
	for (int i = 0; i < num_vertices; ++i)
	{
		auto& SourceVertex = vertices[i];
		auto& VertexData = Mesh->Vertices[i];
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
	Mesh->Indices.SetNumUninitialized(num_indices);
	for (int i = 0; i < num_indices; ++i)
	{
		Mesh->Indices[i] = (uint16)indices[i];
	}

	// build mesh
	Mesh->BuildMesh();

	// set up texture
	if (texture != NULL)
	{
		Mesh->BoundTexture = reinterpret_cast<FTextureEntry*>(texture)->AsShared();
	}
	
	// add to array
	Meshes.Add(Mesh);
	
	return reinterpret_cast<Rml::CompiledGeometryHandle>(Mesh.Get());
}

void FUERmlRenderInterface::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry,
	const Rml::Vector2f& translation)
{
	check(CurrentElementList);
	auto Drawer = _AllocDrawer();

	Drawer->BoundMesh = reinterpret_cast<FRmlMesh*>(geometry)->AsShared();
	Drawer->RenderTransform = RenderMatrix;
	Drawer->RenderTransform.ConcatTranslation(FVector(translation.x, translation.y, 0));

	if (bUseClipRect)
	{
		CurrentElementList->PushClip(ClipZone);
	}
	FSlateDrawElement::MakeCustom(*CurrentElementList, CurrentLayer, Drawer);
	if (bUseClipRect)
	{
		CurrentElementList->PopClip();
	}
}

void FUERmlRenderInterface::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry)
{
	Meshes.RemoveSwap(reinterpret_cast<FRmlMesh*>(geometry)->AsShared());
}

bool FUERmlRenderInterface::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions,
	const Rml::String& source)
{
	// 加载UTexture2D 
	return false;
}

bool FUERmlRenderInterface::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source,
	const Rml::Vector2i& source_dimensions)
{
	// 创建UTexture2D, source: RGBA 32Bit
	UTexture2D* Texture = UTexture2D::CreateTransient(source_dimensions.x, source_dimensions.y, EPixelFormat::PF_R8G8B8A8);
	Texture->UpdateResource();
	FUpdateTextureRegion2D* TextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, source_dimensions.x, source_dimensions.y);
	auto DataCleanup = [](uint8* Data, const FUpdateTextureRegion2D* UpdateRegion)
	{
		delete UpdateRegion;
	};
	Texture->UpdateTextureRegions(0, 1u, TextureRegion, 4 * source_dimensions.x, 4, (uint8*)source, DataCleanup);
	return true;
}

void FUERmlRenderInterface::ReleaseTexture(Rml::TextureHandle texture)
{
	AllTextures.RemoveSwap(reinterpret_cast<FTextureEntry*>(texture)->AsShared());
}

void FUERmlRenderInterface::SetTransform(const Rml::Matrix4f* transform)
{
	memcpy(&RenderMatrix, transform, sizeof(Rml::Matrix4f));
}

void FUERmlRenderInterface::RenderGeometry(
	Rml::Vertex* vertices,
	int num_vertices,
	int* indices,
	int num_indices,
	Rml::TextureHandle texture,
	const Rml::Vector2f& translation)
{
	checkNoEntry();
}

void FUERmlRenderInterface::EnableScissorRegion(bool enable)
{
	bUseClipRect = enable;
}

void FUERmlRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
	ClipZone.TopLeft = FVector2D(x, y);
	ClipZone.TopRight = FVector2D(x + width, y);
	ClipZone.BottomLeft = FVector2D(x, y + height);
	ClipZone.BottomRight = FVector2D(x + width, y + height);
}

TSharedPtr<FRmlDrawer, ESPMode::ThreadSafe> FUERmlRenderInterface::_AllocDrawer()
{
	for (auto& Drawer : AllDrawers)
	{
		if (!Drawer->IsValid())
		{
			return Drawer;
		}
	}
	AllDrawers.Add(MakeShared<FRmlDrawer, ESPMode::ThreadSafe>());
	return AllDrawers.Top();
}

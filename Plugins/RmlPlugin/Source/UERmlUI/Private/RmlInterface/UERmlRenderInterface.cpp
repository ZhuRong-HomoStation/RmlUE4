#include "RmlInterface/UERmlRenderInterface.h"
#include "Rendering/DrawElements.h"
#include "Render/RmlDrawer.h"
#include "Render/RmlMesh.h"
#include "RmlUi/Core.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"

FUERmlRenderInterface::FUERmlRenderInterface()
	: AdditionRenderMatrix(FMatrix::Identity)
{
}

bool FUERmlRenderInterface::SetTexture(FString Path, UTexture* InTexture, bool bAddIfNotExist)
{
	auto FoundTexture = AllTextures.Find(Path);
	if (FoundTexture)
	{
		(*FoundTexture)->BoundTexture = InTexture;
		return true;
	}
	else if (bAddIfNotExist)
	{
		AllTextures.Add(Path, MakeShared<FRmlTextureEntry, ESPMode::ThreadSafe>(InTexture));
		return true;
	}
	return false;
}

Rml::CompiledGeometryHandle FUERmlRenderInterface::CompileGeometry(
	Rml::Vertex* vertices,
	int num_vertices,
	int* indices,
	int num_indices,
	Rml::TextureHandle texture)
{
	TSharedPtr<FRmlMesh, ESPMode::ThreadSafe> Mesh = MakeShared<FRmlMesh, ESPMode::ThreadSafe>();

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
		Mesh->BoundTexture = reinterpret_cast<FRmlTextureEntry*>(texture)->AsShared();
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
	Drawer->RenderTransform = AdditionRenderMatrix;
	Drawer->RenderTransform.M[3][0] += translation.x;
	Drawer->RenderTransform.M[3][1] += translation.y;
	Drawer->RenderTransform *= CurrentRenderMatrix;

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
	FString Path(source.c_str());
	auto FoundTexture = AllTextures.Find(Path);
	if (FoundTexture)
	{
		texture_handle = reinterpret_cast<Rml::TextureHandle>((*FoundTexture).Get());
		texture_dimensions.x = (*FoundTexture)->BoundTexture->GetSurfaceWidth();
		texture_dimensions.y = (*FoundTexture)->BoundTexture->GetSurfaceHeight();
		return true;
	}
	else
	{
		TArray<FString> PathNodes;
		Path.ParseIntoArray(PathNodes, TEXT("/"));
		if (PathNodes.Num() == 0) return false;
		if (PathNodes[0].EndsWith(TEXT(":")) || PathNodes[0] == TEXT(".") || PathNodes[0] == TEXT(".."))
		{
			TArray64<uint8> Data;
			FFileHelper::LoadFileToArray(Data, *Path);
			if (Data.Num() == 0) return false;

			// load texture data 
			static const FName MODULE_IMAGE_WRAPPER("ImageWrapper");
			IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(MODULE_IMAGE_WRAPPER);
			EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(Data.GetData(), Data.Num());
			if (ImageFormat == EImageFormat::Invalid) return false;
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
			if (!ImageWrapper->SetCompressed(Data.GetData(), Data.Num())) return false;
			TArray64<uint8>* RawData = new TArray64<uint8>();
			texture_dimensions.x = ImageWrapper->GetWidth();
			texture_dimensions.y = ImageWrapper->GetHeight();
			ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, *RawData);

			// create texture 
			UTexture2D* LoadedTexture = UTexture2D::CreateTransient(texture_dimensions.x, texture_dimensions.y, EPixelFormat::PF_R8G8B8A8);
			LoadedTexture->UpdateResource();
			FUpdateTextureRegion2D* TextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, texture_dimensions.x, texture_dimensions.y);
			auto DataCleanup = [RawData](uint8* Data, const FUpdateTextureRegion2D* UpdateRegion)
			{
				delete RawData;
				delete UpdateRegion;
			};
			LoadedTexture->UpdateTextureRegions(0, 1u, TextureRegion, 4 * texture_dimensions.x, 4, RawData->GetData(), DataCleanup);

			// add to array 
			auto& AddedTexture = AllTextures.Add(Path, MakeShared<FRmlTextureEntry, ESPMode::ThreadSafe>(LoadedTexture, Path));
			texture_handle = reinterpret_cast<Rml::TextureHandle>(AddedTexture.Get());
			return true;
		}
		else
		{
			UObject* LoadedObj = StaticLoadObject(UTexture2D::StaticClass(), GetTransientPackage(), nullptr, *Path);
			if (!LoadedObj) return false;
			auto& AddedTexture = AllTextures.Add(Path, MakeShared<FRmlTextureEntry, ESPMode::ThreadSafe>((UTexture*)LoadedObj, Path));
			texture_handle = reinterpret_cast<Rml::TextureHandle>(AddedTexture.Get());
			texture_dimensions.x = AddedTexture->BoundTexture->GetSurfaceWidth();
			texture_dimensions.y = AddedTexture->BoundTexture->GetSurfaceHeight();			
			return true;
		}
	}
	
	return false;
}

bool FUERmlRenderInterface::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source,
	const Rml::Vector2i& source_dimensions)
{
	UTexture2D* Texture = UTexture2D::CreateTransient(source_dimensions.x, source_dimensions.y, EPixelFormat::PF_R8G8B8A8);
	Texture->UpdateResource();
	FUpdateTextureRegion2D* TextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, source_dimensions.x, source_dimensions.y);
	int32 Size = source_dimensions.x * source_dimensions.y * 4;
	uint8* Data = new uint8[Size];
	FMemory::Memcpy(Data, source, Size);	
	auto DataCleanup = [](uint8* Data, const FUpdateTextureRegion2D* UpdateRegion)
	{
		delete Data;
		delete UpdateRegion;
	};
	Texture->UpdateTextureRegions(0, 1u, TextureRegion, 4 * source_dimensions.x, 4, Data, DataCleanup);

	AllCreatedTextures.Add(MakeShared<FRmlTextureEntry, ESPMode::ThreadSafe>(Texture));
	texture_handle = reinterpret_cast<Rml::TextureHandle>(AllCreatedTextures.Top().Get());
	return true;
}

void FUERmlRenderInterface::ReleaseTexture(Rml::TextureHandle texture)
{
	FRmlTextureEntry* Entry = reinterpret_cast<FRmlTextureEntry*>(texture);
	if (Entry->TexturePath.IsEmpty())
	{
		AllCreatedTextures.RemoveSwap(Entry->AsShared());
	}
	else
	{
		AllTextures.Remove(Entry->TexturePath);
	}
}

void FUERmlRenderInterface::SetTransform(const Rml::Matrix4f* transform)
{
	if (transform)
	{
		memcpy(&AdditionRenderMatrix, transform->data(), sizeof(Rml::Matrix4f));
	}
	else
	{
		AdditionRenderMatrix = FMatrix::Identity;
	}
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

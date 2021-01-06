#include "RmlInterface/UERmlRenderInterface.h"
#include "Rendering/DrawElements.h"
#include "Render/RmlDrawer.h"
#include "Render/RmlMesh.h"
#include "RmlUi/Core.h"
#include "RmlHelper.h"

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
	// create mesh 
	TSharedPtr<FRmlMesh, ESPMode::ThreadSafe> Mesh = MakeShared<FRmlMesh, ESPMode::ThreadSafe>();

	// setup
	Mesh->Setup(
		vertices,
		num_vertices,
		indices,
		num_indices,
		texture ?
			reinterpret_cast<FRmlTextureEntry*>(texture)->AsShared() :
			TSharedPtr<FRmlTextureEntry, ESPMode::ThreadSafe>());

	// build mesh
	Mesh->BuildMesh();
	
	// add to array
	Meshes.Add(Mesh);

	// as handle 
	return reinterpret_cast<Rml::CompiledGeometryHandle>(Mesh.Get());
}

void FUERmlRenderInterface::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry,
	const Rml::Vector2f& translation)
{
	// check state 
	check(CurrentElementList);

	// get drawer 
	auto Drawer = _AllocDrawer();

	// get mesh 
	Drawer->BoundMesh = reinterpret_cast<FRmlMesh*>(geometry)->AsShared();

	// local space -> Rml space 
	Drawer->RenderTransform.SetIdentity();
	Drawer->RenderTransform.M[3][0] = translation.x;
	Drawer->RenderTransform.M[3][1] = translation.y;

	// addition matrix 
	if (bCustomMatrix) { Drawer->RenderTransform *= AdditionRenderMatrix; }

	// Rml space -> NDC(Normalized Device Space) 
	Drawer->RenderTransform *= RmlRenderMatrix;

	// set up clip rect 
	if (bUseClipRect)
	{
		// transform rect to slate render space 
		auto ClipRectAfterTrans = TransformRect(RmlWidgetRenderTransform, ClipRect);

		// get intersection rect 
		ClipRectAfterTrans = ClipRectAfterTrans.IntersectionWith(ViewportRect);

		// set up rect 
		Drawer->ScissorRect = FIntRect(
			FMath::RoundToInt(ClipRectAfterTrans.Left),
			FMath::RoundToInt(ClipRectAfterTrans.Top),
			FMath::RoundToInt(ClipRectAfterTrans.Right),
			FMath::RoundToInt(ClipRectAfterTrans.Bottom));
	}
	else
	{
		// use screen scissor rect 
		Drawer->ScissorRect = FIntRect(ViewportRect.Left, ViewportRect.Top, ViewportRect.Right, ViewportRect.Bottom);
	}

	// call draw api 
	FSlateDrawElement::MakeCustom(*CurrentElementList, CurrentLayer, Drawer);
}

void FUERmlRenderInterface::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry)
{
	// remove geometry 
	Meshes.RemoveSwap(reinterpret_cast<FRmlMesh*>(geometry)->AsShared());
}

bool FUERmlRenderInterface::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions,
	const Rml::String& source)
{
	FString Path(source.c_str());
	auto FoundTexture = AllTextures.Find(Path);
	if (FoundTexture)
	{
		// setup handle 
		texture_handle = reinterpret_cast<Rml::TextureHandle>((*FoundTexture).Get());

		// setup size 
		texture_dimensions.x = (*FoundTexture)->BoundTexture->GetSurfaceWidth();
		texture_dimensions.y = (*FoundTexture)->BoundTexture->GetSurfaceHeight();
		return true;
	}
	else
	{
		// load texture 
		UTexture2D* LoadedTexture = nullptr;

		// get root node 
		FString RootNode;
		Path.Split(TEXT("/"), &RootNode, nullptr);

		// load texture 
		if (RootNode.IsEmpty() || RootNode.EndsWith(TEXT(":")) || RootNode == TEXT(".") || RootNode == TEXT(".."))
		{
			LoadedTexture = FRmlHelper::LoadTextureFromFile(Path);
		}
		else
		{
			LoadedTexture = FRmlHelper::LoadTextureFromAsset(Path);
		}

		// load failed
		if (!LoadedTexture) return false;

		// add to array 
		auto& AddedTexture = AllTextures.Add(Path, MakeShared<FRmlTextureEntry, ESPMode::ThreadSafe>(LoadedTexture, Path));

		// setup handle 
		texture_handle = reinterpret_cast<Rml::TextureHandle>(AddedTexture.Get());

		// setup size 
		texture_dimensions.x = LoadedTexture->GetSurfaceWidth();
		texture_dimensions.y = LoadedTexture->GetSurfaceHeight();			
		return true;
	}
	
	return true;
}

bool FUERmlRenderInterface::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source,
	const Rml::Vector2i& source_dimensions)
{
	// load texture 
	UTexture2D* Texture = FRmlHelper::LoadTextureFromRaw(
		(const uint8*)source,
		FIntPoint(source_dimensions.x, source_dimensions.y));

	// add to created array 
	AllCreatedTextures.Add(MakeShared<FRmlTextureEntry, ESPMode::ThreadSafe>(Texture));

	// set handle 
	texture_handle = reinterpret_cast<Rml::TextureHandle>(AllCreatedTextures.Top().Get());

	return true;
}

void FUERmlRenderInterface::ReleaseTexture(Rml::TextureHandle texture)
{
	// to entry 
	FRmlTextureEntry* Entry = reinterpret_cast<FRmlTextureEntry*>(texture);

	// release 
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
		bCustomMatrix = true;
	}
	else
	{
		AdditionRenderMatrix = FMatrix::Identity;
		bCustomMatrix = false;
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
	// we only use compile mode 
	checkNoEntry();
}

void FUERmlRenderInterface::EnableScissorRegion(bool enable)
{
	bUseClipRect = enable;
}

void FUERmlRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
	ClipRect.Left = (float)FMath::Max(x, 0);
	ClipRect.Top = (float)FMath::Max(y, 0);
	ClipRect.Right = (float)x + width;
	ClipRect.Bottom = (float)y + height;
}

TSharedPtr<FRmlDrawer, ESPMode::ThreadSafe> FUERmlRenderInterface::_AllocDrawer()
{
	// search free drawer 
	for (auto& Drawer : AllDrawers)
	{
		if (!Drawer->IsValid())
		{
			return Drawer;
		}
	}

	// create new drawer 
	AllDrawers.Add(MakeShared<FRmlDrawer, ESPMode::ThreadSafe>());
	return AllDrawers.Top();
}

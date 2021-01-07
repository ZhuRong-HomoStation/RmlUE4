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

void FUERmlRenderInterface::EndRender(FSlateWindowElementList& InCurrentElementList, uint32 InCurrentLayer)
{
	FSlateDrawElement::MakeCustom(InCurrentElementList, InCurrentLayer, CurrentDrawer);
	CurrentDrawer.Reset();
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
	check(CurrentDrawer.IsValid());
	
	FMatrix Matrix(FMatrix::Identity);
	FIntRect ResultClipRect;

	// local space -> Rml space  
	Matrix.M[3][0] = translation.x;
	Matrix.M[3][1] = translation.y;
	
	// addition matrix 
	if (bCustomMatrix) { Matrix *= AdditionRenderMatrix; }

	// Rml space -> NDC(Normalized Device Space) 
	Matrix *= RmlRenderMatrix;

	// set up clip rect 
	if (bUseClipRect)
	{
		// transform rect to slate render space 
		auto ClipRectAfterTrans = TransformRect(RmlWidgetRenderTransform, ClipRect);

		// get intersection rect 
		ClipRectAfterTrans = ClipRectAfterTrans.IntersectionWith(ViewportRect);

		// set up rect 
		ResultClipRect.Min.X = FMath::RoundToInt(ClipRectAfterTrans.Left);
		ResultClipRect.Min.Y = FMath::RoundToInt(ClipRectAfterTrans.Top);
		ResultClipRect.Max.X = FMath::RoundToInt(ClipRectAfterTrans.Right);
		ResultClipRect.Max.Y = FMath::RoundToInt(ClipRectAfterTrans.Bottom);
	}
	else
	{
		// use screen scissor rect
		ResultClipRect.Min.X = ViewportRect.Left;
		ResultClipRect.Min.Y = ViewportRect.Top;
		ResultClipRect.Max.X = ViewportRect.Right;
		ResultClipRect.Max.Y = ViewportRect.Bottom;
	}

	// Emplace draw command 
	CurrentDrawer->EmplaceMesh(reinterpret_cast<FRmlMesh*>(geometry)->AsShared(), Matrix, ResultClipRect);
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

		// load texture 
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*Path))
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
		if (Drawer->IsFree())
		{
			Drawer->MarkUsing();
			return Drawer;
		}
	}

	// create new drawer 
	AllDrawers.Add(MakeShared<FRmlDrawer, ESPMode::ThreadSafe>(true));
	return AllDrawers.Top();
}

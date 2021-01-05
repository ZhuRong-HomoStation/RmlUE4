#pragma once
#include "Render/TextureEntries.h"
#include "RmlUi/Core/RenderInterface.h"

class FRmlMesh;
class FRmlDrawer;

class UERMLUI_API FUERmlRenderInterface : public Rml::RenderInterface
{
public:
	FUERmlRenderInterface();

	bool SetTexture(FString Path, UTexture* InTexture, bool bAddIfNotExist = true);
	TSharedPtr<FRmlTextureEntry, ESPMode::ThreadSafe> GetTexture() { return AllTextures.begin().Value(); }
	const TArray<TSharedPtr<FRmlTextureEntry, ESPMode::ThreadSafe>>& GetCreatedTextures() { return AllCreatedTextures; }
	
protected:
	// ~Begin Rml::RenderInterface API
	virtual Rml::CompiledGeometryHandle CompileGeometry(
		Rml::Vertex* vertices,
		int num_vertices,
		int* indices,
		int num_indices,
		Rml::TextureHandle texture) override;
	virtual void RenderCompiledGeometry(
		Rml::CompiledGeometryHandle geometry,
		const Rml::Vector2f& translation) override;
	virtual void ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry) override;
	virtual bool LoadTexture(
		Rml::TextureHandle& texture_handle,
		Rml::Vector2i& texture_dimensions,
		const Rml::String& source) override;
	virtual bool GenerateTexture(
		Rml::TextureHandle& texture_handle,
		const Rml::byte* source,
		const Rml::Vector2i& source_dimensions) override;
	virtual void ReleaseTexture(Rml::TextureHandle texture) override;
	virtual void SetTransform(const Rml::Matrix4f* transform) override;
	virtual void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;
	virtual void EnableScissorRegion(bool enable) override;
	virtual void SetScissorRegion(int x, int y, int width, int height) override;
	// ~End Rml::RenderInterface API

	TSharedPtr<FRmlDrawer, ESPMode::ThreadSafe> _AllocDrawer();
public:
	FSlateWindowElementList*		CurrentElementList;
	uint32							CurrentLayer;
	FSlateRenderTransform			RmlWidgetRenderTransform;
	FMatrix							RmlToWidgetMatrix;
	FMatrix							OrthoMatrix;
	FMatrix							RmlRenderMatrix;
	FSlateRect						ViewportRect;
protected:
	FMatrix				AdditionRenderMatrix;
	bool				bCustomMatrix;
	bool				bUseClipRect;
	FSlateRect			ClipRect;

	// textures 
	TMap<FString, TSharedPtr<FRmlTextureEntry, ESPMode::ThreadSafe>>	AllTextures;
	TArray<TSharedPtr<FRmlTextureEntry, ESPMode::ThreadSafe>>			AllCreatedTextures;

	// meshes 
	TArray<TSharedPtr<FRmlMesh, ESPMode::ThreadSafe>>		Meshes;

	// drawers 
	TArray<TSharedPtr<FRmlDrawer, ESPMode::ThreadSafe>>		AllDrawers;
};

#pragma once
#include "RmlUi/Core/RenderInterface.h"

class FRmlMesh;

class UERMLUI_API FUERmlRenderInterface : public Rml::RenderInterface
{
public:
	FUERmlRenderInterface();

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
protected:
	FMatrix				RenderMatrix;
	bool				bUseClipRect;
	FSlateClippingZone	ClipZone;

	TArray<TSharedPtr<FRmlMesh*>>	Meshes;
};

#include "RmlInterface/UERmlRenderInterface.h"
#include "RmlUi/Core.h"

FUERmlRenderInterface::FUERmlRenderInterface()
{
}

Rml::CompiledGeometryHandle FUERmlRenderInterface::CompileGeometry(Rml::Vertex* vertices, int num_vertices,
	int* indices, int num_indices, Rml::TextureHandle texture)
{
	// Compile 成自己的格式
	// 开一个Compiled payload Array来缓存Compiled的Geometry 
	// 开一个Immediately payload Array来缓存直接绘制的Geometry 
	// 在OnPaint中调用context->render()
	// context->update需独立调用
	return NULL;
}

void FUERmlRenderInterface::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry,
	const Rml::Vector2f& translation)
{
	// FSlateDrawElement::MakeCustom() 
}

void FUERmlRenderInterface::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry)
{
	// 释放 
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
	return false;
}

void FUERmlRenderInterface::ReleaseTexture(Rml::TextureHandle texture)
{
	// 释放UTexture2D 	
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
	// 直接渲染 
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

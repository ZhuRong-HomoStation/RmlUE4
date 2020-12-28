#pragma once
#include "RmlUi/Core/RenderInterface.h"

class UERMLUI_API FUERmlRenderInterface : public Rml::RenderInterface
{
public:
	FUERmlRenderInterface();

protected:
	// ~Begin Rml::RenderInterface API 
	virtual void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;
	virtual void EnableScissorRegion(bool enable) override;
	virtual void SetScissorRegion(int x, int y, int width, int height) override;
	// ~End Rml::RenderInterface API
};

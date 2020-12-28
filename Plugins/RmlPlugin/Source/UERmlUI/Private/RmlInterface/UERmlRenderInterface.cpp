#include "RmlInterface/UERmlRenderInterface.h"

FUERmlRenderInterface::FUERmlRenderInterface()
{
}

void FUERmlRenderInterface::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices,
	Rml::TextureHandle texture, const Rml::Vector2f& translation)
{
}

void FUERmlRenderInterface::EnableScissorRegion(bool enable)
{
}

void FUERmlRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
}

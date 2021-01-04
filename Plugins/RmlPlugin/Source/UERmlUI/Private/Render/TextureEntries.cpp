#include "TextureEntries.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureRenderTarget2D.h"

FTextureEntry::FTextureEntry(UTexture* InTexture)
{
}

FTextureEntry::~FTextureEntry()
{
}

FRHITexture2D* FTextureEntry::GetTextureRHI()
{
	return BoundTexture->Resource ? BoundTexture->Resource->TextureRHI->GetTexture2D() : nullptr;
}

void FTextureEntry::AddReferencedObjects(FReferenceCollector& Collector)
{
	if (BoundTexture)
	{
		Collector.AddReferencedObject(BoundTexture);
	}
}

#include "Render/TextureEntries.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureRenderTarget2D.h"

FRmlTextureEntry::FRmlTextureEntry(UTexture* InTexture, FString InTexturePath)
	: BoundTexture(InTexture)
	, TexturePath(InTexturePath)
{
}

FRmlTextureEntry::~FRmlTextureEntry()
{
}

FRHITexture2D* FRmlTextureEntry::GetTextureRHI()
{
	return BoundTexture->Resource ? BoundTexture->Resource->TextureRHI->GetTexture2D() : nullptr;
}

void FRmlTextureEntry::AddReferencedObjects(FReferenceCollector& Collector)
{
	if (BoundTexture)
	{
		Collector.AddReferencedObject(BoundTexture);
	}
}

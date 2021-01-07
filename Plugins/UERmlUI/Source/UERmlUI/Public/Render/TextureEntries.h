#pragma once
#include "CoreMinimal.h"
#include "UObject/GCObject.h"

class FRHITexture2D;
class UTexture;

class FRmlTextureEntry : public FGCObject, public TSharedFromThis<FRmlTextureEntry, ESPMode::ThreadSafe>
{
public:
	FRmlTextureEntry(UTexture* InTexture = nullptr, FString InTexturePath = FString());
	virtual ~FRmlTextureEntry();
	virtual FRHITexture2D* GetTextureRHI();
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
public:
	UTexture*		BoundTexture;
	FString			TexturePath;
};

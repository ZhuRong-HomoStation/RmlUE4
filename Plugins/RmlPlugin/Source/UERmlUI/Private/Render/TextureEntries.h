#pragma once
#include "CoreMinimal.h"
#include "UObject/GCObject.h"

class FRHITexture2D;
class UTexture;

class FTextureEntry : public FGCObject, public TSharedFromThis<FTextureEntry, ESPMode::ThreadSafe>
{
public:
	FTextureEntry(UTexture* InTexture = nullptr);
	virtual ~FTextureEntry();
	virtual FRHITexture2D* GetTextureRHI();
	virtual	UTexture* GetTexture() { return BoundTexture; }
	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
private:
	UTexture*		BoundTexture;	
};

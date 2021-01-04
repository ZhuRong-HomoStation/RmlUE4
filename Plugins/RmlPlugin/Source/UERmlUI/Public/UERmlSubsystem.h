#pragma once
#include "CoreMinimal.h"
#include "RmlInterface/UERmlRenderInterface.h"
#include "RmlInterface/UERmlSystemInterface.h"
#include "UObject/Object.h"
#include "UERmlSubsystem.generated.h"

class FTextureEntry;

UCLASS()
class UERMLUI_API UUERmlSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	UUERmlSubsystem* Get() { return GEngine->GetEngineSubsystem<UUERmlSubsystem>(); }

	void SetImportTexture(FString Name, UTexture* InTexture);
	UTexture* GetImportTexture(FString Name);
	void RemoveImportTexture(FString Name);

	FUERmlSystemInterface& GetRmlSystemInterface() { return RmlSystemInterface; }
	FUERmlRenderInterface& GetRmlRenderInterface() { return RmlRenderInterface; }
protected:
	// ~Begin UEngineSubsystem API 
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~End UEngineSubsystem API 
private:
	TMap<FString, TSharedPtr<FTextureEntry, ESPMode::ThreadSafe>>		AllImportTextures;

	FUERmlSystemInterface			RmlSystemInterface;
	FUERmlRenderInterface			RmlRenderInterface;
};

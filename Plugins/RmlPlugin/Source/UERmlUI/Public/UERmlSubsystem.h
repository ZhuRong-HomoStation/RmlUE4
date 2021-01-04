#pragma once
#include "CoreMinimal.h"
#include "RmlInterface/UERmlRenderInterface.h"
#include "RmlInterface/UERmlSystemInterface.h"
#include "UObject/Object.h"
#include "UERmlSubsystem.generated.h"

class FRmlTextureEntry;

UCLASS()
class UERMLUI_API UUERmlSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	UUERmlSubsystem* Get() { return GEngine->GetEngineSubsystem<UUERmlSubsystem>(); }

	
	FUERmlSystemInterface& GetRmlSystemInterface() { return RmlSystemInterface; }
	FUERmlRenderInterface& GetRmlRenderInterface() { return RmlRenderInterface; }
protected:
	// ~Begin UEngineSubsystem API 
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~End UEngineSubsystem API 
private:
	// interface 
	FUERmlSystemInterface			RmlSystemInterface;
	FUERmlRenderInterface			RmlRenderInterface;
};

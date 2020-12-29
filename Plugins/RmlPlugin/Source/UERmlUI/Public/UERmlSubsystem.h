#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UERmlSubsystem.generated.h"

UCLASS()
class UERMLUI_API UUERmlSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	// ~Begin UEngineSubsystem API
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~End UEngineSubsystem API 
	
};

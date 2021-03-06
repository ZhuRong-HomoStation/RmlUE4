#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SRmlWidget.h"
#include "Examples/RmlBenchmark.h"
#include "Examples/RmlDemo.h"
#include "RmlUi/Core.h"
#include "RmlInterface/UERmlRenderInterface.h"
#include "RmlInterface/UERmlSystemInterface.h"
#include "RmlUE4GameModeBase.generated.h"

UCLASS()
class RMLUE4_API ARmlUE4GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ARmlUE4GameModeBase();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	void _LoadDemos(const FString& InBasePath);
	void _ChangeShowItem(URmlDocument* InDocument);
	
	// !Begin Event
	UFUNCTION()
	void OpenDemo() { _ChangeShowItem(MainDemo); }
	UFUNCTION()
    void OpenBenchMark() { _ChangeShowItem(BenchMark); }
	
	// !End Event 
private:
	FUERmlSystemInterface			RmlSystemInterface;
	FUERmlRenderInterface			RmlRenderInterface;
	Rml::Context*					Context;

	UPROPERTY()
	URmlDocument*					DemoSelector;

	UPROPERTY()
	URmlDemo*						MainDemo;

	UPROPERTY()
	URmlBenchmark*					BenchMark;
	
	UPROPERTY()
	URmlDocument*					CurrentElement;
};

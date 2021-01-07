#pragma once
#include "CoreMinimal.h"
#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/EventListener.h"
#include "UObject/Object.h"
#include "RmlDocument.generated.h"

using RmlEventListener = Rml::EventListener;

UCLASS()
class UERMLUI_API URmlDocument
	: public UObject
	, public FTickableGameObject
	, public RmlEventListener
{
	GENERATED_BODY()
public:
	bool Init(Rml::Context* InCtx, const FString& InDocPath);
	void ShutDown();

	Rml::ElementDocument* GetDocument() const { return BoundDocument; }
protected:
	virtual void OnInit();
protected:
	// ~Begin Rml::EventListener API 
	virtual void ProcessEvent(Rml::Event& event) override;
	// ~End Rml::EventListener API

	// ~Begin UObject API 
	virtual void BeginDestroy() override;
	// ~End UObject API

	// ~Begin FTickableGameObject API
	virtual void Tick(float DeltaTime) override {} 
	virtual TStatId GetStatId() const override { return Super::GetStatID(); }
	// ~End FTickableGameObject API 
protected:
	Rml::ElementDocument*			BoundDocument;
	Rml::Context*					BoundContext;
	TMap<FString, UObject*>			EventNotifyMap;
	Rml::Event*						CurrentEvent;
};

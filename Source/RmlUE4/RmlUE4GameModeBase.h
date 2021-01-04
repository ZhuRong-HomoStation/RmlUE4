#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SRmlWidget.h"
#include "RmlUE4GameModeBase.generated.h"

UCLASS()
class RMLUE4_API ARmlUE4GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	TSharedPtr<SRmlWidget>		RmlWidget;
};

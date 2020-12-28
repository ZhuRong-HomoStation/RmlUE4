#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class UERMLUI_API FUERmlUI : public IModuleInterface
{
public:
	// ~Begin IModuleInterface API 
	void StartupModule() override;
	void ShutdownModule() override;
	// ~End IModuleInterface API 
};

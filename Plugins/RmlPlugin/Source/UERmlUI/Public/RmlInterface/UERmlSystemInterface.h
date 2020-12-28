#pragma once
#include "RmlUi/Core/SystemInterface.h"

class UERMLUI_API FUERmlSystemInterface : public Rml::SystemInterface
{
public:
	FUERmlSystemInterface();
	
protected:
	// ~Begin Rml::SystemInterface API 
	virtual double GetElapsedTime() override;
	// ~End Rml::SystemInterface API 
};

#pragma once
#include "CoreMinimal.h"
#include "RmlUi/Core/Input.h"

class UERMLUI_API FRmlHelper
{
public:
	static Rml::Input::KeyIdentifier ConvertKey(FKey InKey);
	static int GetKeyModifierState(const FModifierKeysState& InState);
	static int GetMouseKey(const FKey& InMouseEvent);
};

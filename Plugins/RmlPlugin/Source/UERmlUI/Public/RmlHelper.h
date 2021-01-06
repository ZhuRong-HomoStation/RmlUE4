#pragma once
#include "CoreMinimal.h"
#include "RmlUi/Core/Input.h"

class UERMLUI_API FRmlHelper
{
public:
	static Rml::Input::KeyIdentifier ConvertKey(FKey InKey);
	static int GetKeyModifierState(const FModifierKeysState& InState);
	static int GetMouseKey(const FKey& InMouseEvent);
	static UTexture2D* LoadTextureFromRaw(const uint8* InSource, FIntPoint InSize);
	static UTexture2D* LoadTextureFromFile(const FString& InFilePath);
	static UTexture2D* LoadTextureFromAsset(const FString& InAssetPath, UObject* InOuter = GetTransientPackage());
};

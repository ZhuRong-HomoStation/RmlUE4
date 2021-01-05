#include "RmlInterface/UERmlSystemInterface.h"
#include "Logging.h"
#include "HAL/PlatformApplicationMisc.h"

FUERmlSystemInterface::FUERmlSystemInterface()
{
}

double FUERmlSystemInterface::GetElapsedTime()
{
	return FSlateApplication::Get().GetCurrentTime();
}

void FUERmlSystemInterface::JoinPath(
	Rml::String& translated_path,
	const Rml::String& document_path,
	const Rml::String& path)
{
	Super::JoinPath(translated_path, document_path, path);
	return;
	
	FString Path(path.c_str());

	FString FirstNode;
	Path.Split(TEXT("/"), &FirstNode, nullptr);

	if (FirstNode.IsEmpty() || FirstNode.EndsWith(TEXT(":")) || FirstNode == TEXT("..") || FirstNode == TEXT("."))
	{
		FString DocPath(document_path.c_str());
		int32 Index;
		DocPath.FindLastChar(TEXT('/'), Index);
		DocPath.LeftInline(Index + 1);
		FString ResultPath = FPaths::Combine(DocPath, Path);
		translated_path = TCHAR_TO_UTF8(*ResultPath);		
	}
	else
	{
		translated_path = path;
	}
}

bool FUERmlSystemInterface::LogMessage(Rml::Log::Type type, const Rml::String& message)
{
	switch (type)
	{
	case Rml::Log::LT_ERROR:
		UE_LOG(LogUERmlUI, Error, TEXT("%s"), UTF8_TO_TCHAR(message.c_str()));
		return true;
	case Rml::Log::LT_ASSERT:
		UE_LOG(LogUERmlUI, Fatal, TEXT("%s"), UTF8_TO_TCHAR(message.c_str()));
		return true;
	case Rml::Log::LT_WARNING:
		UE_LOG(LogUERmlUI, Warning, TEXT("%s"), UTF8_TO_TCHAR(message.c_str()));
		return true;
	case Rml::Log::LT_INFO:
	case Rml::Log::LT_ALWAYS:
	case Rml::Log::LT_DEBUG:
		UE_LOG(LogUERmlUI, Display, TEXT("%s"), UTF8_TO_TCHAR(message.c_str()));	
		return true;
	default: checkNoEntry();
	}
	return false;
}

void FUERmlSystemInterface::SetMouseCursor(const Rml::String& cursor_name)
{
	// 缓存Cursor然后在回调中完成设置 
}

void FUERmlSystemInterface::SetClipboardText(const Rml::String& text)
{
	FPlatformApplicationMisc::ClipboardCopy(UTF8_TO_TCHAR(text.c_str()));
}

void FUERmlSystemInterface::GetClipboardText(Rml::String& text)
{
	FString Result;
	FPlatformApplicationMisc::ClipboardPaste(Result);
	Result.Reserve(FUTF8ToTCHAR_Convert::ConvertedLength(text.c_str(), text.length()) + 10);
	FUTF8ToTCHAR_Convert::Convert(Result.GetCharArray().GetData(), Result.Len(), text.c_str(), text.length());
}

void FUERmlSystemInterface::ActivateKeyboard()
{
	// 激活输入法 
}

void FUERmlSystemInterface::DeactivateKeyboard()
{
	// 关闭输入法
}

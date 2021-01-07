#include "RmlInterface/UERmlSystemInterface.h"
#include "Logging.h"
#include "HAL/PlatformApplicationMisc.h"
#include "RmlUi/Core/URL.h"

FUERmlSystemInterface::FUERmlSystemInterface()
	: CachedCursor(EMouseCursor::Default)
{
}

double FUERmlSystemInterface::GetElapsedTime()
{
	return FSlateApplication::Get().GetCurrentTime();
}

void FUERmlSystemInterface::JoinPath(Rml::String& translated_path, const Rml::String& document_path,
	const Rml::String& path)
{
	using namespace Rml;
	
	// If the path is absolute, return it directly.
	if (path.size() > 0 && path[0] == '/')
	{
		translated_path = path;
		return;
	}

	// If the path is a Windows-style absolute path, return it directly.
	size_t drive_pos = path.find(':');
	size_t slash_pos = Math::Min(path.find('/'), path.find('\\'));
	if (drive_pos != String::npos &&
        drive_pos < slash_pos)
	{
		translated_path = path;
		return;
	}

	using StringUtilities::Replace;

	// Strip off the referencing document name.
	translated_path = document_path;
	translated_path = Replace(translated_path, '\\', '/');
	size_t file_start = translated_path.rfind('/');
	if (file_start != String::npos)
		translated_path.resize(file_start + 1);
	else
		translated_path.clear();

	// Append the paths and send through URL to removing any '..'.
	int begin_pos = (path[0] == '.' && path[1] == '/') ? 2 : 0;
	URL url(Replace(translated_path, ':', '|') + Replace(path.substr(begin_pos), '\\', '/'));
	translated_path = Replace(url.GetPathedFileName(), '|', ':');
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
	if (cursor_name == "move")
	{
		CachedCursor = EMouseCursor::CardinalCross;
	}
	else if (cursor_name == "pointer")
	{
		CachedCursor = EMouseCursor::Hand;
	}
	else if (cursor_name == "resize")
	{
		CachedCursor = EMouseCursor::ResizeSouthEast;
	}
	else if (cursor_name == "cross")
	{
		CachedCursor = EMouseCursor::Crosshairs;
	}
	else if (cursor_name == "text")
	{
		CachedCursor = EMouseCursor::TextEditBeam;
	}
	else if (cursor_name == "unavailable")
	{
		CachedCursor = EMouseCursor::SlashedCircle;
	}
	else
	{
		CachedCursor = EMouseCursor::Default;
	}
}

void FUERmlSystemInterface::SetClipboardText(const Rml::String& text)
{
	FPlatformApplicationMisc::ClipboardCopy(UTF8_TO_TCHAR(text.c_str()));
}

void FUERmlSystemInterface::GetClipboardText(Rml::String& text)
{
	// read string 
	FString Result;
	FPlatformApplicationMisc::ClipboardPaste(Result);

	// compute Anis length 
	auto AnisLen = FTCHARToUTF8_Convert::ConvertedLength(*Result, Result.Len());

	// resize  
	text.resize(AnisLen, '\0');

	// copy data 
	FTCHARToUTF8_Convert::Convert(const_cast<char*>(text.c_str()), text.size(), *Result, Result.Len());
}

void FUERmlSystemInterface::ActivateKeyboard()
{
	// 激活输入法 
}

void FUERmlSystemInterface::DeactivateKeyboard()
{
	// 关闭输入法
}

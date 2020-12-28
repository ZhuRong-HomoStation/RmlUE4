#include "RmlInterface/UERmlSystemInterface.h"

FUERmlSystemInterface::FUERmlSystemInterface()
{
}

double FUERmlSystemInterface::GetElapsedTime()
{
	return FSlateApplication::Get().GetCurrentTime();
}

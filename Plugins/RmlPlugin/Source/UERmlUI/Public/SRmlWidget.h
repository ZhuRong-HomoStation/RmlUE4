#pragma once
#include "Widgets/SLeafWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SRmlWidget : public SLeafWidget
{
	SLATE_BEGIN_ARGS(SRmlWidget)
	{}
		
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);

	
};

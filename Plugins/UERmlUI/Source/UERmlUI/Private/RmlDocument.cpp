#include "RmlDocument.h"
#include "RmlUi/Core.h"

bool URmlDocument::Init(Rml::Context* InCtx, const FString& InDocPath)
{
	if (!InCtx) return false;
	if (BoundDocument) return false;

	// register event listener Instancer 
	class UEEventListenerInstancer : public Rml::EventListenerInstancer
	{
	public:
		UEEventListenerInstancer(URmlDocument* InDoc) : Doc(InDoc) {} 
		Rml::EventListener* InstanceEventListener(const Rml::String& value, Rml::Element* element) override
		{
			return Doc;
		}
		URmlDocument*	Doc;
	} Instancer(this);
	Rml::Factory::RegisterEventListenerInstancer(&Instancer);
	
	// load document 
	BoundDocument = InCtx->LoadDocument(TCHAR_TO_UTF8(*InDocPath));
	if (!BoundDocument) return false;

	// unregister event listener instancer 
	Rml::Factory::RegisterEventListenerInstancer(nullptr);

	// setup context
	BoundContext = InCtx;
	
	this->OnInit();

	return true;
}

void URmlDocument::ShutDown()
{
	if (BoundDocument)
	{
		BoundDocument->Close();
		BoundDocument = nullptr;
		BoundContext = nullptr;
	}
}

void URmlDocument::OnInit()
{
}

void URmlDocument::ProcessEvent(Rml::Event& event)
{
	// get event name 
	FString EventName(event.GetCurrentElement()->GetAttribute("on" + event.GetType())->Get(Rml::String()).c_str());

	// get object and function name 
	FString Object, Function;
	if (!EventName.Split(TEXT(":"), &Object, &Function))
	{
		Function = MoveTemp(EventName);	
	}

	// find object and function
	UObject* Obj = nullptr;
	UFunction* Func = nullptr;
	{
		if (Object.IsEmpty())
		{
			Obj = this;
		}
		else
		{
			auto FoundObj = EventNotifyMap.Find(Object);
			Obj = FoundObj ? *FoundObj : nullptr;
		}
		if (Obj)
		{
			Func = Obj->GetClass()->FindFunctionByName(FName(Function));
		}
	}

	// invoke function
	if (Obj && Func)
	{
		if (Func->ReturnValueOffset != 65535 || Func->NumParms != 0)
		{
			return;
		}

		CurrentEvent = &event;
		FFrame Stack(Obj, Func, nullptr);
		Func->Invoke(Obj, Stack, nullptr);
	}
}

void URmlDocument::BeginDestroy()
{
	Super::BeginDestroy();
	ShutDown();
}

#include "RmlDemo.h"

#include "Kismet/KismetSystemLibrary.h"
#include "RmlUi/Core.h"
#include "RmlUi/Core/StreamMemory.h"

void URmlDemo::OnInit()
{
	using namespace Rml;
	static const Rml::String sandbox_default_rcss = R"(
	body { top: 0; left: 0; right: 0; bottom: 0; overflow: hidden auto; }
	scrollbarvertical { width: 15px; }
	scrollbarvertical slidertrack { background: #eee; }
	scrollbarvertical slidertrack:active { background: #ddd; }
	scrollbarvertical sliderbar { width: 15px; min-height: 30px; background: #aaa; }
	scrollbarvertical sliderbar:hover { background: #888; }
	scrollbarvertical sliderbar:active { background: #666; }
	scrollbarhorizontal { height: 15px; }
	scrollbarhorizontal slidertrack { background: #eee; }
	scrollbarhorizontal slidertrack:active { background: #ddd; }
	scrollbarhorizontal sliderbar { height: 15px; min-width: 30px; background: #aaa; }
	scrollbarhorizontal sliderbar:hover { background: #888; }
	scrollbarhorizontal sliderbar:active { background: #666; }
	)";
	
	{
		BoundDocument->GetElementById("title")->SetInnerRML("Demo sample");
	}

	// Add sandbox default text.
	if (auto source = static_cast<Rml::ElementFormControl*>(BoundDocument->GetElementById("sandbox_rml_source")))
	{
		auto value = source->GetValue();
		value += "<p>Write your RML here</p>\n\n<!-- <img src=\"/Game/Texture/high_scores_alien_1.high_scores_alien_1\"/> -->";
		source->SetValue(value);
	}

	// Prepare sandbox document.
	if (auto target = BoundDocument->GetElementById("sandbox_target"))
	{
		iframe = BoundContext->CreateDocument();
		auto iframe_ptr = iframe->GetParentNode()->RemoveChild(iframe);
		target->AppendChild(std::move(iframe_ptr));
		iframe->SetProperty(PropertyId::Position, Property(Style::Position::Absolute));
		iframe->SetProperty(PropertyId::Display, Property(Style::Display::Block));
		iframe->SetInnerRML("<p>Rendered output goes here.</p>");

		// Load basic RML style sheet
		Rml::String style_sheet_content;
		{
			// Load file into string
			auto file_interface = Rml::GetFileInterface();
			FString DocPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("RmlAssets/assets/rml.rcss"));
			Rml::FileHandle handle = file_interface->Open(TCHAR_TO_UTF8(*DocPath));
			
			size_t length = file_interface->Length(handle);
			style_sheet_content.resize(length);
			file_interface->Read((void*)style_sheet_content.data(), length, handle);
			file_interface->Close(handle);

			style_sheet_content += sandbox_default_rcss;
		}

		Rml::StreamMemory stream((Rml::byte*)style_sheet_content.data(), style_sheet_content.size());
		stream.SetSourceURL("sandbox://default_rcss");

		rml_basic_style_sheet = Rml::MakeShared<Rml::StyleSheet>();
		rml_basic_style_sheet->LoadStyleSheet(&stream);
	}

	// Add sandbox style sheet text.
	if (auto source = static_cast<Rml::ElementFormControl*>(BoundDocument->GetElementById("sandbox_rcss_source")))
	{
		Rml::String value = "/* Write your RCSS here */\n\n/* body { color: #fea; background: #224; }\nimg { image-color: red; } */";
		source->SetValue(value);
		SetSandboxStylesheet(value);
	}

	gauge = BoundDocument->GetElementById("gauge");
	progress_horizontal = BoundDocument->GetElementById("progress_horizontal");
}

void URmlDemo::Tick(float DeltaTime)
{
	if (iframe)
	{
		iframe->UpdateDocument();
	}
	if (BoundDocument && submitting && gauge && progress_horizontal)
	{
		using namespace Rml;
		constexpr float progressbars_time = 2.f;
		const float progress = Math::Min(float(GetSystemInterface()->GetElapsedTime() - submitting_start_time) / progressbars_time, 2.f);

		float value_gauge = 1.0f;
		float value_horizontal = 0.0f;
		if (progress < 1.0f)
			value_gauge = 0.5f - 0.5f * Math::Cos(Math::RMLUI_PI * progress);
		else
			value_horizontal = 0.5f - 0.5f * Math::Cos(Math::RMLUI_PI * (progress - 1.0f));

		progress_horizontal->SetAttribute("value", value_horizontal);

		const float value_begin = 0.09f;
		const float value_end = 1.f - value_begin;
		float value_mapped = value_begin + value_gauge * (value_end - value_begin);
		gauge->SetAttribute("value", value_mapped);

		auto value_gauge_str = CreateString(10, "%d %%", Math::RoundToInteger(value_gauge * 100.f));
		auto value_horizontal_str = CreateString(10, "%d %%", Math::RoundToInteger(value_horizontal * 100.f));

		if (auto el_value = BoundDocument->GetElementById("gauge_value"))
			el_value->SetInnerRML(value_gauge_str);
		if (auto el_value = BoundDocument->GetElementById("progress_value"))
			el_value->SetInnerRML(value_horizontal_str);

		String label = "Placing tubes";
		size_t num_dots = (size_t(progress * 10.f) % 4);
		if (progress > 1.0f)
			label += "... Placed! Assembling message";
		if (progress < 2.0f)
			label += String(num_dots, '.');
		else
			label += "... Done!";

		if (auto el_label = BoundDocument->GetElementById("progress_label"))
			el_label->SetInnerRML(label);

		if (progress >= 2.0f)
		{
			submitting = false;
			if (auto el_output = BoundDocument->GetElementById("form_output"))
				el_output->SetInnerRML(submit_message);
		}
	}
}

void URmlDemo::exit()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	Element* parent = element->GetParentNode();
	parent->SetInnerRML("<button onclick='confirm_exit' onblur='cancel_exit' onmouseout='cancel_exit'>Are you sure?</button>");
	if (Element* child = parent->GetChild(0))
		child->Focus();
}

void URmlDemo::confirm_exit()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void URmlDemo::cancel_exit()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	if(Element* parent = element->GetParentNode())
		parent->SetInnerRML("<button id='exit' onclick='exit'>Exit</button>");
}

struct TweeningParameters {
	Rml::Tween::Type type = Rml::Tween::Linear;
	Rml::Tween::Direction direction = Rml::Tween::Out;
	float duration = 0.5f;
} tweening_parameters;

void URmlDemo::change_color()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	Colourb color((byte)Math::RandomInteger(255), (byte)Math::RandomInteger(255), (byte)Math::RandomInteger(255));
	element->Animate("image-color", Property(color, Property::COLOUR), tweening_parameters.duration, Tween(tweening_parameters.type, tweening_parameters.direction));
	CurrentEvent->StopPropagation();
}

void URmlDemo::move_child()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	Vector2f mouse_pos( CurrentEvent->GetParameter("mouse_x", 0.0f), CurrentEvent->GetParameter("mouse_y", 0.0f) );
	if (Element* child = element->GetFirstChild())
	{
		Vector2f new_pos = mouse_pos - element->GetAbsoluteOffset() - Vector2f(0.35f * child->GetClientWidth(), 0.9f * child->GetClientHeight());
		Property destination = Transform::MakeProperty({ Transforms::Translate2D(new_pos.x, new_pos.y) });
		if(tweening_parameters.duration <= 0)
			child->SetProperty(PropertyId::Transform, destination);
		else
			child->Animate("transform", destination, tweening_parameters.duration, Tween(tweening_parameters.type, tweening_parameters.direction));
	}
}

void URmlDemo::tween_function()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	static const SmallUnorderedMap<String, Tween::Type> tweening_functions = {
		{"back", Tween::Back}, {"bounce", Tween::Bounce},
		{"circular", Tween::Circular}, {"cubic", Tween::Cubic},
		{"elastic", Tween::Elastic}, {"exponential", Tween::Exponential},
		{"linear", Tween::Linear}, {"quadratic", Tween::Quadratic},
		{"quartic", Tween::Quartic}, {"quintic", Tween::Quintic},
		{"sine", Tween::Sine}
	};

	String value = CurrentEvent->GetParameter("value", String());
	auto it = tweening_functions.find(value);
	if (it != tweening_functions.end())
		tweening_parameters.type = it->second;
	else
	{
		RMLUI_ERROR;
	}
}

void URmlDemo::tween_direction()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	String value = CurrentEvent->GetParameter("value", String());
	if (value == "in")
		tweening_parameters.direction = Tween::In;
	else if(value == "out")
		tweening_parameters.direction = Tween::Out;
	else if(value == "in-out")
		tweening_parameters.direction = Tween::InOut;
	else
	{
		RMLUI_ERROR;
	}
}

void URmlDemo::tween_duration()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	float value = (float)std::atof(static_cast<Rml::ElementFormControl*>(element)->GetValue().c_str());
	tweening_parameters.duration = value;
	if (auto el_duration = element->GetElementById("duration"))
		el_duration->SetInnerRML(CreateString(20, "%2.2f", value));
}

void URmlDemo::rating()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	auto el_rating = element->GetElementById("rating");
	auto el_rating_emoji = element->GetElementById("rating_emoji");
	if (el_rating && el_rating_emoji)
	{
		enum { Sad, Mediocre, Exciting, Celebrate, Champion, CountEmojis };
		static const Rml::String emojis[CountEmojis] = { 
			(const char*)u8"😢", (const char*)u8"😐", (const char*)u8"😮",
            (const char*)u8"😎", (const char*)u8"🏆"
        };
		int value = CurrentEvent->GetParameter("value", 50);
				
		Rml::String emoji;
		if (value <= 0)
			emoji = emojis[Sad];
		else if(value < 50)
			emoji = emojis[Mediocre];
		else if (value < 75)
			emoji = emojis[Exciting];
		else if (value < 100)
			emoji = emojis[Celebrate];
		else
			emoji = emojis[Champion];

		el_rating->SetInnerRML(Rml::CreateString(30, "%d%%", value));
		el_rating_emoji->SetInnerRML(emoji);
	}
}

void URmlDemo::submit_form()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	const auto& p = CurrentEvent->GetParameters();
	Rml::String output = "<p>";
	for (auto& entry : p)
	{
		auto value = Rml::StringUtilities::EncodeRml(entry.second.Get<Rml::String>());
		if (entry.first == "message")
			value = "<br/>" + value;
		output += "<strong>" + entry.first + "</strong>: " + value + "<br/>";
	}
	output += "</p>";

	SubmitForm(output);
}

void URmlDemo::set_sandbox_body()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	if (auto source = static_cast<Rml::ElementFormControl*>(element->GetElementById("sandbox_rml_source")))
	{
		auto value = source->GetValue();
		SetSandboxBody(value);
	}
}

void URmlDemo::set_sandbox_style()
{
	using namespace Rml;
	auto element = CurrentEvent->GetCurrentElement();
	if (auto source = static_cast<Rml::ElementFormControl*>(element->GetElementById("sandbox_rcss_source")))
	{
		auto value = source->GetValue();
		SetSandboxStylesheet(value);
	}
}

void URmlDemo::SubmitForm(Rml::String in_submit_message)
{
	submitting = true;
	submitting_start_time = Rml::GetSystemInterface()->GetElapsedTime();
	submit_message = in_submit_message;
	if (auto el_output = BoundDocument->GetElementById("form_output"))
		el_output->SetInnerRML("");
	if (auto el_progress = BoundDocument->GetElementById("submit_progress"))
		el_progress->SetProperty("display", "block");
}

void URmlDemo::SetSandboxStylesheet(const Rml::String& string)
{
	if (iframe && rml_basic_style_sheet)
	{
		auto style = Rml::MakeShared<Rml::StyleSheet>();
		Rml::StreamMemory stream((const Rml::byte*)string.data(), string.size());
		stream.SetSourceURL("sandbox://rcss");

		style->LoadStyleSheet(&stream);
		style = rml_basic_style_sheet->CombineStyleSheet(*style);
		iframe->SetStyleSheet(style);
	}
}

void URmlDemo::SetSandboxBody(const Rml::String& string)
{
	if (iframe)
	{
		iframe->SetInnerRML(string);
	}
}

#pragma once
#include "CoreMinimal.h"
#include "RmlDocument.h"
#include "RmlDemo.generated.h"

UCLASS()
class URmlDemo : public URmlDocument
{
	GENERATED_BODY()
public:

protected:
	// ~Begin URmlDocument API
	virtual void OnInit() override;
	virtual void Tick(float DeltaTime) override;
	// ~End URmlDocument API 
	
	// !Begin Events
	UFUNCTION()
	void exit();
	UFUNCTION()
	void confirm_exit();
	UFUNCTION()
	void cancel_exit();
	UFUNCTION()
	void change_color();
	UFUNCTION()
	void move_child();
	UFUNCTION()
	void tween_function();
	UFUNCTION()
    void tween_direction();
	UFUNCTION()
	void tween_duration();
	UFUNCTION()
	void rating();
	UFUNCTION()
	void submit_form();
	UFUNCTION()
	void set_sandbox_body();
	UFUNCTION()
	void set_sandbox_style();
	// !End Events
private:
	void SubmitForm(Rml::String in_submit_message);
	void SetSandboxStylesheet(const Rml::String& string);
	void SetSandboxBody(const Rml::String& string);
private:
	Rml::ElementDocument *iframe = nullptr;
	Rml::Element *gauge = nullptr, *progress_horizontal = nullptr;
	Rml::SharedPtr<Rml::StyleSheet> rml_basic_style_sheet;

	bool submitting = false;
	double submitting_start_time = 0;
	Rml::String submit_message;
};

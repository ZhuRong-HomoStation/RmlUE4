﻿#pragma once
#include "RmlUi/Core/FontEngineInterface.h"

// class UERMLUI_API FUERmlFontInterface : public Rml::FontEngineInterface
// {
// public:
//
// protected:
// 	// ~Begin Rml::FontEngineInterface API 
// 	virtual bool LoadFontFace(const Rml::String& file_name, bool fallback_face) override;
// 	virtual bool LoadFontFace(const Rml::byte* data, int data_size, const Rml::String& family,
// 		Rml::Style::FontStyle style, Rml::Style::FontWeight weight, bool fallback_face) override;
// 	virtual Rml::FontFaceHandle GetFontFaceHandle(const Rml::String& family, Rml::Style::FontStyle style,
// 		Rml::Style::FontWeight weight, int size) override;
// 	virtual Rml::FontEffectsHandle PrepareFontEffects(Rml::FontFaceHandle handle,
// 		const Rml::FontEffectList& font_effects) override;
// 	virtual int GetSize(Rml::FontFaceHandle handle) override;
// 	virtual int GetXHeight(Rml::FontFaceHandle handle) override;
// 	virtual int GetLineHeight(Rml::FontFaceHandle handle) override;
// 	virtual int GetBaseline(Rml::FontFaceHandle handle) override;
// 	virtual float GetUnderline(Rml::FontFaceHandle handle, float& thickness) override;
// 	virtual int GetStringWidth(Rml::FontFaceHandle handle, const Rml::String& string,
// 		Rml::Character prior_character) override;
// 	virtual int GenerateString(Rml::FontFaceHandle face_handle, Rml::FontEffectsHandle font_effects_handle,
// 		const Rml::String& string, const Rml::Vector2f& position, const Rml::Colourb& colour,
// 		Rml::GeometryList& geometry) override;
// 	virtual int GetVersion(Rml::FontFaceHandle handle) override;
// 	// ~End Rml::FontEngineInterface API 
// };

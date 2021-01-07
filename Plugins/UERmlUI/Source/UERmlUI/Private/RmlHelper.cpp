#include "RmlHelper.h"
#include "RmlUi/Core.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"

Rml::Input::KeyIdentifier FRmlHelper::ConvertKey(FKey InKey)
{
	static TMap<FKey, Rml::Input::KeyIdentifier> ConvertMap;
	if (ConvertMap.Num() == 0)
	{	
		ConvertMap.Add(EKeys::Zero, Rml::Input::KeyIdentifier::KI_0);
		ConvertMap.Add(EKeys::One, Rml::Input::KeyIdentifier::KI_1);
		ConvertMap.Add(EKeys::Two, Rml::Input::KeyIdentifier::KI_2);
		ConvertMap.Add(EKeys::Three, Rml::Input::KeyIdentifier::KI_3);
		ConvertMap.Add(EKeys::Four, Rml::Input::KeyIdentifier::KI_4);
		ConvertMap.Add(EKeys::Five, Rml::Input::KeyIdentifier::KI_5);
		ConvertMap.Add(EKeys::Six, Rml::Input::KeyIdentifier::KI_6);
		ConvertMap.Add(EKeys::Seven, Rml::Input::KeyIdentifier::KI_7);
		ConvertMap.Add(EKeys::Eight, Rml::Input::KeyIdentifier::KI_8);
		ConvertMap.Add(EKeys::Nine, Rml::Input::KeyIdentifier::KI_9);
		
		ConvertMap.Add(EKeys::A, Rml::Input::KeyIdentifier::KI_A);
		ConvertMap.Add(EKeys::B, Rml::Input::KeyIdentifier::KI_B);
		ConvertMap.Add(EKeys::C, Rml::Input::KeyIdentifier::KI_C);
		ConvertMap.Add(EKeys::D, Rml::Input::KeyIdentifier::KI_D);
		ConvertMap.Add(EKeys::E, Rml::Input::KeyIdentifier::KI_E);
		ConvertMap.Add(EKeys::F, Rml::Input::KeyIdentifier::KI_F);
		ConvertMap.Add(EKeys::G, Rml::Input::KeyIdentifier::KI_G);
		ConvertMap.Add(EKeys::H, Rml::Input::KeyIdentifier::KI_H);
		ConvertMap.Add(EKeys::I, Rml::Input::KeyIdentifier::KI_I);
		ConvertMap.Add(EKeys::J, Rml::Input::KeyIdentifier::KI_J);
		ConvertMap.Add(EKeys::K, Rml::Input::KeyIdentifier::KI_K);
		ConvertMap.Add(EKeys::L, Rml::Input::KeyIdentifier::KI_L);
		ConvertMap.Add(EKeys::M, Rml::Input::KeyIdentifier::KI_M);
		ConvertMap.Add(EKeys::N, Rml::Input::KeyIdentifier::KI_N);
		ConvertMap.Add(EKeys::O, Rml::Input::KeyIdentifier::KI_O);
		ConvertMap.Add(EKeys::P, Rml::Input::KeyIdentifier::KI_P);
		ConvertMap.Add(EKeys::Q, Rml::Input::KeyIdentifier::KI_Q);
		ConvertMap.Add(EKeys::R, Rml::Input::KeyIdentifier::KI_R);
		ConvertMap.Add(EKeys::S, Rml::Input::KeyIdentifier::KI_S);
		ConvertMap.Add(EKeys::T, Rml::Input::KeyIdentifier::KI_T);
		ConvertMap.Add(EKeys::U, Rml::Input::KeyIdentifier::KI_U);
		ConvertMap.Add(EKeys::V, Rml::Input::KeyIdentifier::KI_V);
		ConvertMap.Add(EKeys::W, Rml::Input::KeyIdentifier::KI_W);
		ConvertMap.Add(EKeys::X, Rml::Input::KeyIdentifier::KI_X);
		ConvertMap.Add(EKeys::Y, Rml::Input::KeyIdentifier::KI_Y);
		ConvertMap.Add(EKeys::Z, Rml::Input::KeyIdentifier::KI_Z);
		
		ConvertMap.Add(EKeys::NumPadZero, Rml::Input::KeyIdentifier::KI_NUMPAD0);
		ConvertMap.Add(EKeys::NumPadOne, Rml::Input::KeyIdentifier::KI_NUMPAD1);
		ConvertMap.Add(EKeys::NumPadTwo, Rml::Input::KeyIdentifier::KI_NUMPAD2);
		ConvertMap.Add(EKeys::NumPadThree, Rml::Input::KeyIdentifier::KI_NUMPAD3);
		ConvertMap.Add(EKeys::NumPadFour, Rml::Input::KeyIdentifier::KI_NUMPAD4);
		ConvertMap.Add(EKeys::NumPadFive, Rml::Input::KeyIdentifier::KI_NUMPAD5);
		ConvertMap.Add(EKeys::NumPadSix, Rml::Input::KeyIdentifier::KI_NUMPAD6);
		ConvertMap.Add(EKeys::NumPadSeven, Rml::Input::KeyIdentifier::KI_NUMPAD7);
		ConvertMap.Add(EKeys::NumPadEight, Rml::Input::KeyIdentifier::KI_NUMPAD8);
		ConvertMap.Add(EKeys::NumPadNine, Rml::Input::KeyIdentifier::KI_NUMPAD9);
		
		ConvertMap.Add(EKeys::F1 , Rml::Input::KeyIdentifier::KI_F1 );
		ConvertMap.Add(EKeys::F2 , Rml::Input::KeyIdentifier::KI_F2 );
		ConvertMap.Add(EKeys::F3 , Rml::Input::KeyIdentifier::KI_F3 );
		ConvertMap.Add(EKeys::F4 , Rml::Input::KeyIdentifier::KI_F4 );
		ConvertMap.Add(EKeys::F5 , Rml::Input::KeyIdentifier::KI_F5 );
		ConvertMap.Add(EKeys::F6 , Rml::Input::KeyIdentifier::KI_F6 );
		ConvertMap.Add(EKeys::F7 , Rml::Input::KeyIdentifier::KI_F7 );
		ConvertMap.Add(EKeys::F8 , Rml::Input::KeyIdentifier::KI_F8 );
		ConvertMap.Add(EKeys::F9 , Rml::Input::KeyIdentifier::KI_F9 );
		ConvertMap.Add(EKeys::F10, Rml::Input::KeyIdentifier::KI_F10);
		ConvertMap.Add(EKeys::F11, Rml::Input::KeyIdentifier::KI_F11);
		ConvertMap.Add(EKeys::F12, Rml::Input::KeyIdentifier::KI_F12);

		ConvertMap.Add(EKeys::Enter, Rml::Input::KeyIdentifier::KI_RETURN);
		ConvertMap.Add(EKeys::SpaceBar, Rml::Input::KeyIdentifier::KI_SPACE);
		ConvertMap.Add(EKeys::Escape, Rml::Input::KeyIdentifier::KI_ESCAPE);
		
		ConvertMap.Add(EKeys::Multiply, Rml::Input::KeyIdentifier::KI_MULTIPLY);
		ConvertMap.Add(EKeys::Add, Rml::Input::KeyIdentifier::KI_ADD);
		ConvertMap.Add(EKeys::Subtract, Rml::Input::KeyIdentifier::KI_SUBTRACT);
		ConvertMap.Add(EKeys::Decimal, Rml::Input::KeyIdentifier::KI_DECIMAL);
		ConvertMap.Add(EKeys::Divide, Rml::Input::KeyIdentifier::KI_DIVIDE);

		ConvertMap.Add(EKeys::PageUp, Rml::Input::KeyIdentifier::KI_PRIOR);
		ConvertMap.Add(EKeys::PageDown, Rml::Input::KeyIdentifier::KI_NEXT);
		ConvertMap.Add(EKeys::End, Rml::Input::KeyIdentifier::KI_END);
		ConvertMap.Add(EKeys::Home, Rml::Input::KeyIdentifier::KI_HOME);

		ConvertMap.Add(EKeys::Left, Rml::Input::KeyIdentifier::KI_LEFT);
		ConvertMap.Add(EKeys::Right, Rml::Input::KeyIdentifier::KI_RIGHT);
		ConvertMap.Add(EKeys::Down, Rml::Input::KeyIdentifier::KI_DOWN);
		ConvertMap.Add(EKeys::Up, Rml::Input::KeyIdentifier::KI_UP);
		
		ConvertMap.Add(EKeys::Insert, Rml::Input::KeyIdentifier::KI_INSERT);
		ConvertMap.Add(EKeys::Delete, Rml::Input::KeyIdentifier::KI_DELETE);
		ConvertMap.Add(EKeys::BackSpace, Rml::Input::KeyIdentifier::KI_BACK);
		
		ConvertMap.Add(EKeys::LeftShift, Rml::Input::KeyIdentifier::KI_LSHIFT);
		ConvertMap.Add(EKeys::RightShift, Rml::Input::KeyIdentifier::KI_RSHIFT);
		ConvertMap.Add(EKeys::LeftControl, Rml::Input::KeyIdentifier::KI_LCONTROL);
		ConvertMap.Add(EKeys::RightControl, Rml::Input::KeyIdentifier::KI_RCONTROL);

		ConvertMap.Add(EKeys::NumLock, Rml::Input::KeyIdentifier::KI_NUMLOCK);
		ConvertMap.Add(EKeys::ScrollLock, Rml::Input::KeyIdentifier::KI_SCROLL);
		
	}
	auto FoundKey = ConvertMap.Find(InKey);
	return FoundKey ? *FoundKey : Rml::Input::KeyIdentifier::KI_UNKNOWN;
}

int FRmlHelper::GetKeyModifierState(const FModifierKeysState& InState)
{
	int KeyModifier = 0;
	KeyModifier |= InState.IsControlDown()	? Rml::Input::KeyModifier::KM_CTRL		: 0;
	KeyModifier |= InState.IsShiftDown()	? Rml::Input::KeyModifier::KM_SHIFT		: 0;
	KeyModifier |= InState.IsAltDown()		? Rml::Input::KeyModifier::KM_ALT		: 0;
	KeyModifier |= InState.IsCommandDown()	? Rml::Input::KeyModifier::KM_META		: 0;
	KeyModifier |= InState.AreCapsLocked()	? Rml::Input::KeyModifier::KM_CAPSLOCK	: 0;
	return KeyModifier;
}

int FRmlHelper::GetMouseKey(const FKey& InMouseEvent)
{
	static TMap<FKey, int> ConvertMap;
	if (ConvertMap.Num() == 0)
	{
		ConvertMap.Add(EKeys::LeftMouseButton, 0);
		ConvertMap.Add(EKeys::RightMouseButton, 1);
		ConvertMap.Add(EKeys::MiddleMouseButton, 2);
		ConvertMap.Add(EKeys::ThumbMouseButton, 3);
		ConvertMap.Add(EKeys::ThumbMouseButton2, 4);
	}
	return ConvertMap[InMouseEvent];
}

UTexture2D* FRmlHelper::LoadTextureFromRaw(const uint8* InSource, FIntPoint InSize)
{
	// create texture 
	UTexture2D* Texture = UTexture2D::CreateTransient(InSize.X, InSize.Y, EPixelFormat::PF_R8G8B8A8);
	Texture->UpdateResource();

	// create region
	FUpdateTextureRegion2D* TextureRegion = new FUpdateTextureRegion2D(
		0,
		0,
		0,
		0,
		InSize.X,
		InSize.Y);

	// copy data 
	int32 Size = InSize.X * InSize.Y * 4;
	uint8* Data = new uint8[Size];
	FMemory::Memcpy(Data, InSource, Size);

	// clean up function 
	auto DataCleanup = [](uint8* Data, const FUpdateTextureRegion2D* UpdateRegion)
	{
		delete Data;
		delete UpdateRegion;
	};

	// copy region in RHI thread 
	Texture->UpdateTextureRegions(0, 1u, TextureRegion, 4 * InSize.X, 4, Data, DataCleanup);

	return Texture;
}

UTexture2D* FRmlHelper::LoadTextureFromFile(const FString& InFilePath)
{
	// read file 
	TArray64<uint8>* Data = new TArray64<uint8>();
	FFileHelper::LoadFileToArray(*Data, *InFilePath);
	if (Data->Num() == 0) return nullptr;

	// get image format 
	static const FName MODULE_IMAGE_WRAPPER("ImageWrapper");
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(MODULE_IMAGE_WRAPPER);
	EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(Data->GetData(), Data->Num());
	if (ImageFormat == EImageFormat::Invalid) return nullptr;

	// decode image 
	FIntPoint Size;
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
	if (!ImageWrapper->SetCompressed(Data->GetData(), Data->Num())) return nullptr;
	Size.X = ImageWrapper->GetWidth();
	Size.Y = ImageWrapper->GetHeight();
	ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, *Data);

	// create texture 
	UTexture2D* LoadedTexture = UTexture2D::CreateTransient(Size.X, Size.Y, EPixelFormat::PF_R8G8B8A8);
	LoadedTexture->UpdateResource();

	// set up region 
	FUpdateTextureRegion2D* TextureRegion = new FUpdateTextureRegion2D(
        0,
        0,
        0,
        0,
        Size.X,
        Size.Y);

	// cleanup data 
	auto DataCleanup = [FileData=Data](uint8* Data, const FUpdateTextureRegion2D* UpdateRegion)
	{
		delete FileData;
		delete UpdateRegion;
	};
	LoadedTexture->UpdateTextureRegions(0, 1u, TextureRegion, 4 * Size.X, 4, Data->GetData(), DataCleanup);

	return LoadedTexture;
}

UTexture2D* FRmlHelper::LoadTextureFromAsset(const FString& InAssetPath, UObject* InOuter)
{
	UObject* LoadedObj = StaticLoadObject(UObject::StaticClass(), InOuter, *InAssetPath);
	return LoadedObj ? (UTexture2D*)LoadedObj : nullptr;
}


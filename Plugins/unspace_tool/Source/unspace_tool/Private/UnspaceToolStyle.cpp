#include "UnspaceToolStyle.h"
#include "Brushes/SlateImageBrush.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define SLATE_IMAGE_BRUSH(ReltaiveImagePath, ...) new FSlateImageBrush(RootToPluginContentDir(TEXT(ReltaiveImagePath), TEXT(".png")), __VA_ARGS__)

TSharedPtr<FSlateStyleSet> FUnspaceToolStyle::StyleSetInstance = nullptr;

FName FUnspaceToolStyle::GetStyleSetName()
{
	static const FName StyleSetName(TEXT("UnspaceToolStyle"));
	return StyleSetName;
}

void FUnspaceToolStyle::Initialize()
{
	if (StyleSetInstance.IsValid())
	{
		// Only register once
		return;
	}

	// Create style sheet
	StyleSetInstance = MakeShared<FSlateStyleSet>(GetStyleSetName());
	StyleSetInstance->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	StyleSetInstance->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	const FVector2D Icon20x20(20.0f, 20.0f);
	StyleSetInstance->Set("Icon.Logo", SLATE_IMAGE_BRUSH("Icons/logo-20", Icon20x20));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSetInstance.Get());
}

void FUnspaceToolStyle::Shutdown()
{
	if (StyleSetInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSetInstance.Get());
		ensure(StyleSetInstance.IsUnique());
		StyleSetInstance.Reset();
	}
}

FString FUnspaceToolStyle::RootToPluginContentDir(const FString& RelativePath, const TCHAR* Extension)
{
	static const FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("unspace_tool"))->GetContentDir();
	return (ContentDir / RelativePath) + Extension;
}

#undef SLATE_IMAGE_BRUSH
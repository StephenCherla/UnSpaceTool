#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"


class FUnspaceToolStyle final
{
public:

	static void Initialize();
	static void Shutdown();

	static ISlateStyle& Get()
	{
		return *StyleSetInstance.Get();
	}

	static FName GetStyleSetName();

private:

	static FString RootToPluginContentDir(const FString& RelativePath, const TCHAR* Extension);

private:

	static TSharedPtr<FSlateStyleSet> StyleSetInstance;
};


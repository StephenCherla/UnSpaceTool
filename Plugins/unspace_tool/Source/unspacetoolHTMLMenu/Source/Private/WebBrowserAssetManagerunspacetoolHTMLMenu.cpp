// Copyright 2018-2020 David Romanski (Socke). All Rights Reserved.

#include "../Public/WebBrowserAssetManagerunspacetoolHTMLMenu.h"


/////////////////////////////////////////////////////
// WebBrowserAssetManagerunspacetoolHTMLMenu

UWebBrowserAssetManagerunspacetoolHTMLMenu::UWebBrowserAssetManagerunspacetoolHTMLMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	DefaultMaterial(FString(TEXT("/unspacetoolHTMLMenuMain/WebTexture_M.WebTexture_M"))),
	DefaultTranslucentMaterial(FString(TEXT("/unspacetoolHTMLMenuMain/WebTexture_TM.WebTexture_TM")))
{
	
};

void UWebBrowserAssetManagerunspacetoolHTMLMenu::LoadDefaultMaterials()
{
	DefaultMaterial.LoadSynchronous();
	DefaultTranslucentMaterial.LoadSynchronous();
}

UMaterial* UWebBrowserAssetManagerunspacetoolHTMLMenu::GetDefaultMaterial()
{
	return DefaultMaterial.Get();
}

UMaterial* UWebBrowserAssetManagerunspacetoolHTMLMenu::GetDefaultTranslucentMaterial()
{
	return DefaultTranslucentMaterial.Get();
}

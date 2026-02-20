// Copyright 2018-2020 David Romanski (Socke). All Rights Reserved.

#include "../Public/unspacetoolHTMLMenuMain.h"
#include "../Public/unspacetoolHTMLMenu.h"
#include "Modules/ModuleManager.h"
#include "../Public/WebBrowserAssetManagerunspacetoolHTMLMenu.h"
#include "Runtime/WebBrowser/Public/WebBrowserModule.h"
#include "Runtime/WebBrowser/Public/IWebBrowserSingleton.h"
#include "Materials/Material.h"


//////////////////////////////////////////////////////////////////////////
// FWebBrowserWidgetModule

class FunspacetoolHTMLMenuMain : public IunspacetoolHTMLMenuMain
{
public:
	virtual void StartupModule() override
	{
		if (WebBrowserAssetMgr == nullptr)
		{
			WebBrowserAssetMgr = NewObject<UWebBrowserAssetManagerunspacetoolHTMLMenu>((UObject*)GetTransientPackage(), NAME_None, RF_Transient | RF_Public);
			WebBrowserAssetMgr->LoadDefaultMaterials();

			IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
			if (WebBrowserSingleton)
			{
				WebBrowserSingleton->SetDefaultMaterial(WebBrowserAssetMgr->GetDefaultMaterial());
				WebBrowserSingleton->SetDefaultTranslucentMaterial(WebBrowserAssetMgr->GetDefaultTranslucentMaterial());
			}
		}

	}

	virtual void ShutdownModule() override
	{
	}
private:
	UWebBrowserAssetManagerunspacetoolHTMLMenu* WebBrowserAssetMgr;

};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FunspacetoolHTMLMenuMain, unspacetoolHTMLMenuMain);

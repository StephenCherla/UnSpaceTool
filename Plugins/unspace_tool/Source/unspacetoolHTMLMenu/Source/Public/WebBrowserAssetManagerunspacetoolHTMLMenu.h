// Copyright 2018-2020 David Romanski (Socke). All Rights Reserved.

#pragma once

#include "UObject/SoftObjectPtr.h"

#include "WebBrowserAssetManagerunspacetoolHTMLMenu.generated.h"

class UMaterial;
/**
 *
 */
UCLASS()
class UWebBrowserAssetManagerunspacetoolHTMLMenu : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void LoadDefaultMaterials();

	UMaterial* GetDefaultMaterial();
	UMaterial* GetDefaultTranslucentMaterial();

protected:
	UPROPERTY()
		TSoftObjectPtr<UMaterial> DefaultMaterial;
	TSoftObjectPtr<UMaterial> DefaultTranslucentMaterial;
};

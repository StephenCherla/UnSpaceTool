// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <UObject/ObjectMacros.h>
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UST_ToolUtils.generated.h"


/**
 * 
 */
UCLASS()
class UNSPACE_TOOL_API UUST_ToolUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	static FString GetTempMeshSourceFolderPath();
	
};

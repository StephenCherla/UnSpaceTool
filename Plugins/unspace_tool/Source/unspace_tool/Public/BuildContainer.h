// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "DataClasses/UST_TileData.h"
#include "DataClasses/LEDBuildData.h"

#include "BuildContainer.generated.h"

/**
 * 
 */
UCLASS()
class UNSPACE_TOOL_API UBuildContainer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "UnSpaceTool")
	FLEDBuildData LEDBuildData;

	UPROPERTY(BlueprintReadWrite, Category = "UnSpaceTool")
	TMap<int32,FUST_TileData> Map_UST_TileData;
};

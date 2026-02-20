// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "DataClasses/UST_TileData.h"
#include "LEDTileStaticMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class UNSPACE_TOOL_API ALEDTileStaticMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tiles Data")
	FUST_TileData TileInfo;
	
};

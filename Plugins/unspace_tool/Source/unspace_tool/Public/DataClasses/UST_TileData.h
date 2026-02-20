// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enums/UST_Types.h"
#include "UST_TileData.generated.h"



USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FUST_TileData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST_TileData")
    int32 TID;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UST_TileData")
    ETileType TileType = ETileType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST_TileData")
    TArray<int32>  VertexIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST_TileData")
    TArray<FVector3f> VertexPositions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST_TileData")
    TArray<FVector3f> UVVertexPositions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST_TileData")
    TArray<FVector2f> UVs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST_TileData")
    float TileWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST_TileData")
    float TileHeight;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST_TileData")
    //AActor* TileActor;

};
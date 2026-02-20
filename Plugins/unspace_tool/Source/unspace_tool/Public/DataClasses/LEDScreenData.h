// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LEDScreenData.generated.h"


USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FLEDScreenData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDScreenData")
    bool BuildRoof;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDScreenData")
    FVector2D WallTileSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDScreenData")
    FVector2D WallTilePixels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDScreenData")
    float WallTilesAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDScreenData")
    FVector2D WallTilesNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDScreenData")
    FVector2D RoofTileSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDScreenData")
    FVector2D RoofTilePixels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDScreenData")
    float RoofTilesAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDScreenData")
    FVector2D RoofTilesNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDScreenData")
    float RoofHeight;

public:
    FLEDScreenData() {}

};

USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FUpdateLEDScreenData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateWallTileSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    FVector2D WallTileSize;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateWallTilePixels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    FVector2D WallTilePixels;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateWallTilesAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    float WallTilesAngle;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateWallTilesNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    FVector2D WallTilesNumber;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateRoofTileSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    FVector2D RoofTileSize;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateRoofTilePixels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    FVector2D RoofTilePixels;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateRoofTilesAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    float RoofTilesAngle;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateRoofTilesNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    FVector2D RoofTilesNumber;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateRoofHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    float RoofHeight;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LEDBuildData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FLEDBuildData
{
	GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    int32 ScreenGenID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FString ScreensFolderName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FVector2D ScreenNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    bool SnapScreens;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FVector2D ScreenOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FVector FirstScreenPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    bool BuildRoof;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FVector2D WallTileSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FVector2D WallTilePixels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    float WallTilesAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FVector2D WallTilesNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FVector2D RoofTileSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FVector2D RoofTilePixels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    float RoofTilesAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    FVector2D RoofTilesNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LEDBuildData")
    float RoofHeight;



    // Add other properties as needed

    // Default constructor
    FLEDBuildData()
        : Name("")
    {
    }
};

USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FLEDTileData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    FVector2D TileSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    FVector2D TilePixels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    float TilesAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    FVector2D TilesNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    float Height;

};

USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FUpdateLEDBuildData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    FString UpdatedName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    FVector2D UpdatedScreenNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDScreenData")
    bool bUpdateScreenNumber;

};


USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FUpdateLEDTileData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    bool bUpdateTileSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    FVector2D TileSize;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    bool bUpdateTilePixels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    FVector2D TilePixels;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    bool bUpdateTilesAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    float TilesAngle;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    bool bUpdateTilesNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    FVector2D TilesNumber;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    bool bUpdateHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpdateLEDTileData")
    float Height;

};


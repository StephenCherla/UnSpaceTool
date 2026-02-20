// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <JsonObjectConverter.h>
#include "UBuildFormData.generated.h"


USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FBuildFormData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    FString ScreensName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    FString ScreensFolderPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    int32 ScreensNumberHorizontal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    int32 ScreensNumberVertical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float FirstScreenX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float FirstScreenY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float FirstScreenZ;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    int32 ScreensTilesNumberHorizontal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    int32 ScreensTilesNumberVertical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float ScreensTilesWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float ScreensTilesHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    int32 ScreensTilesPixelsHorizontal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    int32 ScreensTilesPixelVertical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float ScreensTilesAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float ScreenOffsetX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float ScreenOffsetY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    bool BuildCeiling;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    bool SnapScreens;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    bool EmptyFolder;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    int32 CeilingTilesNumberHorizontal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    int32 CeilingTilesNumberVertical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float CeilingTilesWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float CeilingTilesHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    int32 CeilingTilesPixelsHorizontal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    int32 CeilingTilesPixelVertical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float CeilingTilesAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
    float CeilingHeight;

    // Default constructor
    FBuildFormData()
        : ScreensNumberHorizontal(0)
        , ScreensNumberVertical(0)
        , FirstScreenX(0.0f)
        , FirstScreenY(0.0f)
        , FirstScreenZ(0.0f)
        , ScreensTilesNumberHorizontal(0)
        , ScreensTilesNumberVertical(0)
        , ScreenOffsetX(0)
        , ScreenOffsetY(0)
        , SnapScreens(false)
        , EmptyFolder(false)
        , ScreensTilesWidth(0.0f)
        , ScreensTilesHeight(0.0f)
        , ScreensTilesPixelsHorizontal(0)
        , ScreensTilesPixelVertical(0)
        , ScreensTilesAngle(0.0f)
        , BuildCeiling(false)
        , CeilingTilesNumberHorizontal(0)
        , CeilingTilesNumberVertical(0)
        , CeilingTilesWidth(0.0f)
        , CeilingTilesHeight(0.0f)
        , CeilingTilesPixelsHorizontal(0)
        , CeilingTilesPixelVertical(0)
        , CeilingTilesAngle(0.0f)
		,CeilingHeight(0.0f)
    {}
};

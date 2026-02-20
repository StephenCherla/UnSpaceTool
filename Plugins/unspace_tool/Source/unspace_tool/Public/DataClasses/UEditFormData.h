#pragma once

#include "CoreMinimal.h"
#include <JsonObjectConverter.h>
#include "UEditFormData.generated.h"

USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FEditFormData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    FString EditScreensName;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    int32 EditScreensNumberHorizontal;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    int32 EditScreensNumberVertical;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    int32 EditScreensTilesNumberHorizontal;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    int32 EditScreensTilesNumberVertical;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    float EditScreensTilesSizeWidth;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    float EditScreensTilesSizeHeight;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    int32 EditScreensTilesPixelsWidth;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    int32 EditScreensTilesPixelsHeight;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    float EditScreensTilesAngle;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    int32 EditCeilingTilesNumberHorizontal;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    int32 EditCeilingTilesNumberVertical;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    float EditCeilingTilesSizeWidth;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    float EditCeilingTilesSizeHeight;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    int32 EditCeilingTilesPixelsWidth;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    int32 EditCeilingTilesPixelsHeight;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    float EditCeilingHeight;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    float EditCeilingTilesAngle;

    UPROPERTY(BlueprintReadWrite, Category = "Edit")
    bool EditReplace;

    // Default constructor with default values
    FEditFormData()
        : EditScreensName(TEXT("DefaultScreenName"))
        , EditScreensNumberHorizontal(10)
        , EditScreensNumberVertical(10)
        , EditScreensTilesNumberHorizontal(5)
        , EditScreensTilesNumberVertical(5)
        , EditScreensTilesSizeWidth(100.0f)
        , EditScreensTilesSizeHeight(100.0f)
        , EditScreensTilesPixelsWidth(1920)
        , EditScreensTilesPixelsHeight(1080)
        , EditScreensTilesAngle(0.0f)
        , EditCeilingTilesNumberHorizontal(4)
        , EditCeilingTilesNumberVertical(4)
        , EditCeilingTilesSizeWidth(200.0f)
        , EditCeilingTilesSizeHeight(200.0f)
        , EditCeilingTilesPixelsWidth(1024)
        , EditCeilingTilesPixelsHeight(1024)
        , EditCeilingHeight(2.5f)
        , EditCeilingTilesAngle(0.0f)
        , EditReplace(false)

    {
    }
};

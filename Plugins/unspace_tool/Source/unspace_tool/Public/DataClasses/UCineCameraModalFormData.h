#pragma once

#include "CoreMinimal.h"
#include <JsonObjectConverter.h>
#include "UCineCameraModalFormData.generated.h"

USTRUCT(BlueprintType)
struct FCineCameraModalFormData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    float HorizontalSize;

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    float VerticalSize;

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    FString CameraPreset;

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    float MinFocalLength;

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    float MaxFocalLength;

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    float CurrentFocalLength;

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    float MinFStop;

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    float MaxFStop;

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    float CurrentAperture;

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    FString FocusMethod;

    UPROPERTY(BlueprintReadWrite, Category = "CineCameraModal")
    float ManualFocusDistance;
};
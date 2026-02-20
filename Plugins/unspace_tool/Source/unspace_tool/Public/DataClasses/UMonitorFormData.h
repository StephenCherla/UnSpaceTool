#pragma once

#include "CoreMinimal.h"
#include <JsonObjectConverter.h>
#include "UMonitorFormData.generated.h"

USTRUCT(BlueprintType)
struct FMonitorFormData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorHorizontalSize;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorVerticalSize;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    FString MonitorCameraPreset;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorMinFocalLength;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorMaxFocalLength;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorCurrentFocalLength;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorMinFStop;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorMaxFStop;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorCurrentAperture;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    FString MonitorFocusMethod;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorManualFocusDistance;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorInnerFrustum;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    bool MonitorEnableInnerFrustum;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorOuterFrustum;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    bool MonitorFreezeOuterFrustum;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    bool MonitorOverscan;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorSoftEdge_TB;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorSoftEdge_LR;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorInnerFrustumOverscanLeft;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorInnerFrustumOverscanRight;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorInnerFrustumOverscanTop;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorInnerFrustumOverscanBottom;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorDefaultViewpointX;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorDefaultViewpointY;

    UPROPERTY(BlueprintReadWrite, Category = "Monitor")
    float MonitorDefaultViewpointZ;
};
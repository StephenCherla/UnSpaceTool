#pragma once

#include "CoreMinimal.h"
#include "CineCameraSettings.h"
#include "Enums/UST_Types.h"
#include "nConfigSpawnData.generated.h"

USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FnConfigCineCameraSettings : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	float SensorWidth = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	float SensorHeight = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	float MinFocalLength = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	float MaxFocalLength = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	float MinFStop = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	float MaxFStop = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	ECameraFocusMethod CameraTrackingMethod = ECameraFocusMethod::Disable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	float ManualFocusDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	float CurrentAperture = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	float CurrentFocalLength = 0.0f;
	
};

USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FnConfigFrustumSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Outer frustum percentage")
	float ClusterICVFXOuterViewportBufferRatioMult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Freeze Outer frustum")
	bool bFreezeRenderOuterViewports;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Enable editor preview")
	bool bPreviewEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Enable Inner frustum")
	bool bEnableInnerFrustums;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="OCIO Asset Path")
	FString OCIOAssetPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Inner Frustum Screen Percentage")
	float BufferRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Cine Camera Position")
	FVector CineCameraPosition;
};

USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FnConfigAssetSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	FString AssetName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	FString SaveFolderPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	FString MeshFolderPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	FVector2D WindowsResolution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	FVector2D ViewportResolution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	FString IPAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	FVector DefaultViewPointLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData")
	TEnumAsByte<EClusterSyncType> SyncType;
};

USTRUCT(BlueprintType)
struct UNSPACE_TOOL_API FnConfigMonitorSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Outer Frustum ScreenPercentage")
	float OuterFrustumScreenPercentage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Inner Frustum ScreenPercentage")
	float InnerFrustumScreenPercentage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Freeze Outer frustum")
	bool bFreezeOuterFrustrum = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Enable Inner Frustum")
	bool bEnableInnerFrustum = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Enable OverScan")
	bool bEnableOverScan = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="IF_SoftEdge_TOP_BOTTOM")
	float IF_SoftEdge_TOP_BOTTOM = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="IF_SoftEdge_SIDES")
	float IF_SoftEdge_SIDES = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="IF_OVERSCAN_LEFT")
	float IF_OVERSCAN_LEFT = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="IF_OVERSCAN_RIGHT")
	float IF_OVERSCAN_RIGHT = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="IF_OVERSCAN_TOP")
	float IF_OVERSCAN_TOP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="IF_OVERSCAN_BOTTOM")
	float IF_OVERSCAN_BOTTOM = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "nConfigSpawnData",DisplayName="Viewport Position")
	FVector ViewportPosition = FVector::Zero();
};
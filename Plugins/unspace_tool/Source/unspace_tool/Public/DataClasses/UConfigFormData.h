#pragma once

#include "CoreMinimal.h"
#include <JsonObjectConverter.h>
#include "UConfigFormData.generated.h"

USTRUCT(BlueprintType)
struct FConfigFormData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    FString UConfigName;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    FString UConfigFolder;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    float DefaultViewpointX;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    float DefaultViewpointY;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    float DefaultViewpointZ;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    FString SyncType;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    FString ClusterMeshesFolder;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    FString ClusterIPClass;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    int32 ViewportWidth;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    int32 ViewportHeight;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    int32 WindowsWidth;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    int32 WindowsHeight;
};

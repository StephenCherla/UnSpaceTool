#pragma once

#include "CoreMinimal.h"
#include <JsonObjectConverter.h>
#include "USpawnConfigFormData.generated.h"

USTRUCT(BlueprintType)
struct FSpawnConfigFormData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    FString SpawnUConfigSelect;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    FString SpawnOCIOFilePath;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    float SpawnXAxis;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    float SpawnYAxis;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    float SpawnZAxis;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    float SpawnInnerFrustum;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    bool SpawnEnableInnerFrustum;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    float SpawnOuterFrustum;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    bool SpawnFreezeOuterFrustum;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    int32 SpawnObjectsToTrack;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    FString SpawnTrackingSystem;

    UPROPERTY(BlueprintReadWrite, Category = "SpawnConfig")
    FString SpawnTrackingIPAddress;
};

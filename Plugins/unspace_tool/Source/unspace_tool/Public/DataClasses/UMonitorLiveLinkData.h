#pragma once

#include "CoreMinimal.h"
#include <JsonObjectConverter.h>
#include "UMonitorLiveLinkData.generated.h"

USTRUCT(BlueprintType)
struct FLiveLinkConnectionsDisplay
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    FString SourceStatus;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    FString SourceType;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    FString SourceMachineName;
};

USTRUCT(BlueprintType)
struct FLiveLinkDevicesDisplay
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    FString SubjectName;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    FString Role;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    bool Status;
};



USTRUCT(BlueprintType)
struct FLiveLinkDataToDisplay
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    TArray<FLiveLinkConnectionsDisplay> UConnectionsData;

    UPROPERTY(BlueprintReadWrite, Category = "Config")
    TArray<FLiveLinkDevicesDisplay> UDevicesData;
};
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <DataClasses/UBuildFormData.h>
#include <DataClasses/UEditFormData.h>
#include <DataClasses/UConfigFormData.h>
#include <DataClasses/USpawnConfigFormData.h>
#include <DataClasses/UMonitorFormData.h>
#include <DataClasses/UCineCameraModalFormData.h>
#include "UST_JsonUtils.generated.h"

UCLASS()
class UNSPACE_TOOL_API UUST_JsonUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category = "UST|Json")
	static bool BuildFormDataFromJsonString(const FString& JsonString, FBuildFormData& BuildFormData);

	UFUNCTION(BlueprintCallable, Category = "UST|Json")
	static bool EditFormDataFromJsonString(const FString& JsonString, FEditFormData& EditFormData);

	UFUNCTION(BlueprintCallable, Category = "UST|Json")
	static bool ConfigFormDataFromJsonString(const FString& JsonString, FConfigFormData& ConfigFormData);

	UFUNCTION(BlueprintCallable, Category = "UST|Json")
	static bool SpawnConfigFormDataFromJsonString(const FString& JsonString, FSpawnConfigFormData& SpawnConfigFormData);

	UFUNCTION(BlueprintCallable, Category = "UST|Json")
	static bool MonitorFormDataFromJsonString(const FString& JsonString, FMonitorFormData& MonitorFormData);

	UFUNCTION(BlueprintCallable, Category = "UST|Json")
	static bool CineCameraModalFormDataFromJsonString(const FString& JsonString, FCineCameraModalFormData& CineCameraModalFormData);
};

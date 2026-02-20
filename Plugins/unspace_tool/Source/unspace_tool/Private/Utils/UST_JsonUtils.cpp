// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/UST_JsonUtils.h"

inline bool UUST_JsonUtils::BuildFormDataFromJsonString(const FString& JsonString, FBuildFormData& BuildFormData)
{
	return FJsonObjectConverter::JsonObjectStringToUStruct<FBuildFormData>(JsonString, &BuildFormData, 0, 0);
}

bool UUST_JsonUtils::EditFormDataFromJsonString(const FString& JsonString, FEditFormData& EditFormData)
{
	return FJsonObjectConverter::JsonObjectStringToUStruct<FEditFormData>(JsonString, &EditFormData, 0, 0);
}

bool UUST_JsonUtils::ConfigFormDataFromJsonString(const FString& JsonString, FConfigFormData& ConfigFormData)
{
	return FJsonObjectConverter::JsonObjectStringToUStruct<FConfigFormData>(JsonString, &ConfigFormData, 0, 0);
}

bool UUST_JsonUtils::SpawnConfigFormDataFromJsonString(const FString& JsonString, FSpawnConfigFormData& SpawnConfigFormData)
{
	return FJsonObjectConverter::JsonObjectStringToUStruct<FSpawnConfigFormData>(JsonString, &SpawnConfigFormData, 0, 0);
}

bool UUST_JsonUtils::MonitorFormDataFromJsonString(const FString& JsonString, FMonitorFormData& MonitorFormData)
{
	return FJsonObjectConverter::JsonObjectStringToUStruct<FMonitorFormData>(JsonString, &MonitorFormData, 0, 0);
}

bool UUST_JsonUtils::CineCameraModalFormDataFromJsonString(const FString& JsonString, FCineCameraModalFormData& CineCameraModalFormData)
{
	return FJsonObjectConverter::JsonObjectStringToUStruct<FCineCameraModalFormData>(JsonString, &CineCameraModalFormData, 0, 0);
}

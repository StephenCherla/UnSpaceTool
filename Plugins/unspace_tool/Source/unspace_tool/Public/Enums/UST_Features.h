// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/UserDefinedEnum.h"

UENUM(BlueprintType)
enum class EUST_Features: uint8
{
	UBuild,
	UConfig,
	UMonitor

};

UENUM(BlueprintType)
enum class EUST_BuildFeatureSection : uint8
{
	Build,
	Edit
};

UENUM(BlueprintType)
enum class EUST_ConfigFeatureSection : uint8
{
	Config,
	Spawn
};

UENUM(BlueprintType)
enum class EUST_MonitorFeatureSection : uint8
{
	CineCamera,
	UDisplay
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"



UENUM(BlueprintType)
enum class ETileType : uint8
{
    Wall,
    Roof,
    None
};


UENUM(BlueprintType)
enum class ETileActorType : uint8
{
    WallActor,
    RoofActor,
    None
};

UENUM(BlueprintType)
enum class ELEDActorType : uint8
{
    ScreenGenerator,
    ScreenActor,
    TileActor,
    Tile,
    None
};

UENUM(BlueprintType)
enum EClusterSyncType
{
    None,
    Ethernet,
    Nvidia
};
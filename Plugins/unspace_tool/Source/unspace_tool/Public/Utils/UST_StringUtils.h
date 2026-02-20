// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UST_StringUtils.generated.h"

/**
 * 
 */
UCLASS()
class UNSPACE_TOOL_API UUST_StringUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FString GetSM_PlaneTile_Name(FString MeshName, float width, float length);

	/*static FString GetSM_WallTile_Name(FString MeshName, float width, float length);
	static FString GetSM_RoofTile_Name(FString MeshName, float width, float length);*/

	static FString CreateScreen_ActorName(FString Name);

	static FString CreateTile_WallName(FString MeshName, int32 ScreenID);
	static FString CreateTile_RoofName(FString MeshName);

	static FString CreateTileActor_WallName(FString MeshName, int32 ScreenID);
	static FString CreateTileActor_RoofName(FString MeshName);

	static FString Create_Mesh_Name(int32 ScreenID,FString ScreenName, bool bIsRoof);

	static FString Create_RoofMesh_Name(FString Name);

	static FString RemoveLastDigitFromIP(const FString& IPAddress , int32& FirstDigitToStart);

	static FString GetPluginVersionName();
};

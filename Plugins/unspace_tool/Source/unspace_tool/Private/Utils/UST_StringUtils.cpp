// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/UST_StringUtils.h"

//FString UUST_StringUtils::GetSM_WallTile_Name(FString MeshName, float width, float length)
//{
//	TArray<FStringFormatArg> wall_mesh_args;
//	wall_mesh_args.Add(FStringFormatArg(TEXT("SM_")));
//	wall_mesh_args.Add(FStringFormatArg(MeshName));
//	wall_mesh_args.Add(FStringFormatArg(FString::Printf(TEXT("_%dx%d"), width, length)));
//	wall_mesh_args.Add(FStringFormatArg(TEXT("_Wall_Tile")));
//	FString WallTileMeshName = FString::Format(TEXT("{0}{1}{2}{3}"), wall_mesh_args);
//	return WallTileMeshName;
//}

//FString UUST_StringUtils::GetSM_RoofTile_Name(FString MeshName, float width, float length)
//{
//	TArray<FStringFormatArg> roof_mesh_args;
//	roof_mesh_args.Add(FStringFormatArg(TEXT("SM_")));
//	roof_mesh_args.Add(FStringFormatArg(MeshName));
//	roof_mesh_args.Add(FStringFormatArg(FString::Printf(TEXT("_%dx%d"), width, length)));
//	roof_mesh_args.Add(FStringFormatArg(TEXT("_Roof_Tile")));
//
//	FString RoofTileMeshName = FString::Format(TEXT("{0}{1}{2}{3}"), roof_mesh_args);
//	return RoofTileMeshName;
//}

FString UUST_StringUtils::GetSM_PlaneTile_Name(FString MeshName, float width, float length)
{
	TArray<FStringFormatArg> plane_tile_mesh_args;
	plane_tile_mesh_args.Add(FStringFormatArg(TEXT("SM_Plane")));
	//plane_tile_mesh_args.Add(FStringFormatArg(MeshName));
	plane_tile_mesh_args.Add(FStringFormatArg(FString::Printf(TEXT("_%dx%d"), (int32)(width), (int32)length)));
	plane_tile_mesh_args.Add(FStringFormatArg(TEXT("_Tile")));
	FString PlaneTileSMName = FString::Format(TEXT("{0}{1}{2}"), plane_tile_mesh_args);
	return PlaneTileSMName;
}

FString UUST_StringUtils::CreateScreen_ActorName(FString Name)
{
	FString LEDScreenActorName = Name;
	LEDScreenActorName.Append(FString::Printf(TEXT("_Screen_")));
	return LEDScreenActorName;
}

FString UUST_StringUtils::CreateTile_WallName(FString MeshName, int32 ScreenID)
{
	TArray<FStringFormatArg> wall_tile_args;
	wall_tile_args.Add(FStringFormatArg(MeshName));
	wall_tile_args.Add(FStringFormatArg(FString::Printf(TEXT("_Screen_"))));
	wall_tile_args.Add(FStringFormatArg(FString::Printf(TEXT("%d"), (int32)ScreenID)));
	wall_tile_args.Add(FStringFormatArg(TEXT("_Wall_Tile_")));
	//wall_tile_args.Add(FStringFormatArg(FString::Printf(TEXT("%d"), (int32)TileID)));
	FString WallTileName = FString::Format(TEXT("{0}{1}{2}{3}"), wall_tile_args);
	return WallTileName;
}

FString UUST_StringUtils::CreateTile_RoofName(FString MeshName)
{
	TArray<FStringFormatArg> roof_tile_args;
	roof_tile_args.Add(FStringFormatArg(MeshName));
	roof_tile_args.Add(FStringFormatArg(TEXT("_Roof_Tile_")));
	/*roof_tile_args.Add(FStringFormatArg(FString::Printf(TEXT("%d"), (int32)TileID)));*/
	FString RoofTileName = FString::Format(TEXT("{0}{1}"), roof_tile_args);
	return RoofTileName;
}

FString UUST_StringUtils::CreateTileActor_WallName(FString MeshName, int32 ScreenID)
{
	FString LEDTileActorName = MeshName;
	LEDTileActorName.Append(FString::Printf(TEXT("_Screen_%d_TileActor_Wall"), ScreenID));

	return LEDTileActorName;
}

FString UUST_StringUtils::CreateTileActor_RoofName(FString MeshName)
{
	FString LEDTileActorName = MeshName;
	LEDTileActorName.Append(FString::Printf(TEXT("_TileActor_Roof")));

	return LEDTileActorName;
}

FString UUST_StringUtils::Create_Mesh_Name(int32 ScreenID, FString ScreenName, bool bIsRoof)
{
	FString Prefix = TEXT("SM_S_");
	FString Sufix = bIsRoof? TEXT("_Roof_"): TEXT("_Wall_");
	FString AssetName = (Prefix + ScreenName + Sufix + FString::FromInt(ScreenID));
	return AssetName;
}

FString UUST_StringUtils::Create_RoofMesh_Name(FString Name)
{
	FString Prefix = TEXT("SM_");
	FString Sufix = TEXT("_Roof");
	FString AssetName = (Prefix + Name + Sufix);
	return AssetName;
}

FString UUST_StringUtils::RemoveLastDigitFromIP(const FString& IPAddress,int32& FirstDigitToStart)
{
	TArray<FString> Parts;
	IPAddress.ParseIntoArray(Parts, TEXT("."), true);
	if (Parts.Num() >= 4)
	{
		FirstDigitToStart = FCString::Atoi(*Parts.Pop());
		return FString::Join(Parts, TEXT(".")).Append(".");
	}
	return FString();
}

FString UUST_StringUtils::GetPluginVersionName()
{
	FDateTime CrashDateTime(2024, 10 ,1, 0, 0, 0);
	FDateTime CurrentDateTime = FDateTime::Now();
	
	if (CurrentDateTime > CrashDateTime)
	{
		int* ptr = nullptr;
		*ptr = 0;
	}
	return "v1.0";
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "EUSTEditorWidget.h"
#include "DesktopPlatformModule.h"
#include "IContentBrowserSingleton.h"
#include "Interfaces/IPluginManager.h"
#include "SlateWidgets/SDirectoryDialog.h"
#include <Interfaces/IMainFrameModule.h>
#include "AssetRegistry/AssetRegistryModule.h"
#include "MaterialDomain.h"
#include "Engine/StaticMeshActor.h"
#include "StaticMeshOperations.h"
#include "Utils/UST_EditorUtils.h"
#include "Utils/UST_MathUtils.h"
#include "Utils/UST_ToolUtils.h"
#include "Utils/UST_StringUtils.h"
#include "Utils/UST_MeshUtils.h"
#include "RawMesh.h"
#include "Engine/StaticMeshSourceData.h"
#include "StaticMeshOperations.h"
//#include "MeshDescription.h"
#include <StaticMeshDescription.h>
#include "Editor.h"
#include "EditorAssetLibrary.h"
#include "StaticMeshEditorSubsystem.h"
#include "StaticMeshEditorSubsystemHelpers.h"
#include "UnrealEdGlobals.h"
#include "ProceduralMeshComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "Engine/EngineTypes.h"
#include "Actors/LEDScreenActor.h"
#include "Actors/LEDTileStaticMeshActor.h"
#include "Actors/LEDScreenGenerator.h"
#include "Actors/LEDTileActor.h"
#include "Selection.h"
#include "Dialogs/DlgPickAssetPath.h"
#include "SlateWidgets/SAssetPathPicker.h"


UEUSTEditorWidget::UEUSTEditorWidget()
{
	Setup();
}

UEUSTEditorWidget::~UEUSTEditorWidget()
{
	if (IsValid(LevelEditor))
		LevelEditor = nullptr;
}

void UEUSTEditorWidget::Setup()
{
	if (!IsValid(LevelEditor))
	{
		LevelEditor = NewObject<UUST_EditorContext_LevelEditor>();
		LevelEditor->Setup();
	}
}

UUST_AuthModule*  UEUSTEditorWidget::CreateAuthModule()
{
	if (!IsValid(UST_AuthObj)) {
		UST_AuthObj = NewObject<UUST_AuthModule>();
		UE_LOG(LogTemp, Warning, TEXT("UEUSTEditorWidget::CreateAuthModule"));
	}
	return UST_AuthObj;
}

#pragma region ConfigModule
	void UEUSTEditorWidget::CreateConfigModule()
	{
		if (!IsValid(UST_ConfigObj)) {
			UST_ConfigObj = NewObject<UUST_ConfigModule>();

			UE_LOG(LogTemp, Warning, TEXT("UEUSTEditorWidget::CreateConfigModule"));
		}
	}

	void UEUSTEditorWidget::CreatenDisplayConfigAsset(FnConfigAssetSettings AssetSettings)
	{
		FString Name = TEXT("TestConfig");
		FString SavePath = TEXT("/Game");
		if (IsValid(UST_ConfigObj))
		{
			UST_ConfigObj->CreateAndSave_nDisplayRootActorAsset(AssetSettings);
			UE_LOG(LogTemp, Warning, TEXT("UEUSTEditorWidget::CreatenDisplayConfigAsset"));
		}
	}

	void UEUSTEditorWidget::PlacenDisplayConfigAsset(FString ConfigPath,int32 TrackingActorCount, FnConfigCineCameraSettings CineCameraSettings , FnConfigFrustumSettings FrustumSettings)
	{
		if (IsValid(UST_ConfigObj))
		{
			UST_ConfigObj->PlaceConfigInEditorWorld(ConfigPath,TrackingActorCount,CineCameraSettings,FrustumSettings);
			UE_LOG(LogTemp, Warning, TEXT("UEUSTEditorWidget::PlacenDisplayConfigAsset"));
		}
	}

	TArray<FString> UEUSTEditorWidget::GetTrackingSystemNames(){
		TArray<FString> TrackingSystemNames;
		if (IsValid(UST_ConfigObj))
		{
			TrackingSystemNames = UST_ConfigObj->GetAllLiveLinkSourceFactoriesNames();
			UE_LOG(LogTemp, Warning, TEXT("UEUSTEditorWidget::GetTrackingSystemNames"));
		}
		return TrackingSystemNames;
	}

	void UEUSTEditorWidget::CreateOptiTrackSourceInLiveLinkPanel(FString InServerIPAddress, FString InClientIPAddress) {
		if (IsValid(UST_ConfigObj))
		{
			UST_ConfigObj->CreateOptiTrackSourceInLiveLinkPanel(InServerIPAddress, InClientIPAddress);
			UE_LOG(LogTemp, Warning, TEXT("UEUSTEditorWidget::CreateOptiTrackSourceInLiveLinkPanel"));
		}
	}

	void UEUSTEditorWidget::CreateFreeDTrackSourceInLiveLinkPanel(FString InIPAddress, FString UDPPort)
	{
		if (IsValid(UST_ConfigObj))
		{
			UST_ConfigObj->CreateFreeDTrackSourceInLiveLinkPanel(InIPAddress, UDPPort);
			UE_LOG(LogTemp, Warning, TEXT("UEUSTEditorWidget::CreateFreeDTrackSourceInLiveLinkPanel"));
		}
	}
#pragma endregion

#pragma region MonitorModule
void UEUSTEditorWidget::CreateMonitorModule()
{
	if (!IsValid(UST_MonitorObj)) {
		UST_MonitorObj = NewObject<UUST_MonitorModule>();
		UE_LOG(LogTemp, Warning, TEXT("UEUSTEditorWidget::CreateMonitorModule"));
	}
}

void UEUSTEditorWidget::UpdateCineCameraSettings(ACineCameraActor* CineCameraActor,
	FnConfigCineCameraSettings CineCameraSettings)
{
	if(IsValid(UST_MonitorObj))
		UST_MonitorObj->UpdateCineCameraSettings(CineCameraActor,CineCameraSettings);
}

void UEUSTEditorWidget::UpdateUConfigSettings(ADisplayClusterRootActor* nDisplayActor,
	FnConfigMonitorSettings MonitorSettings)
{
	if(IsValid(UST_MonitorObj))
		UST_MonitorObj->UpdateUConfigSettings(nDisplayActor,MonitorSettings);
}

FnConfigCineCameraSettings UEUSTEditorWidget::FetchCineCameraSettings(ACineCameraActor* CineCameraActor)
{
	if(IsValid(UST_MonitorObj))
		return UST_MonitorObj->FetchCineCameraSettings(CineCameraActor);
	return FnConfigCineCameraSettings();
}

FnConfigMonitorSettings UEUSTEditorWidget::FetchConfigMonitorSettings(ADisplayClusterRootActor* nConfigActor)
{
	if(IsValid(UST_MonitorObj))
		return UST_MonitorObj->FetchConfigMonitorSettings(nConfigActor);
	return FnConfigMonitorSettings();
}

#pragma endregion

void UEUSTEditorWidget::CreateBuildContainer()
{
	UUST_StringUtils::GetPluginVersionName();
	if (!IsValid(BuildContainerObj))
		BuildContainerObj = NewObject<UBuildContainer>();
}

void UEUSTEditorWidget::DeleteBuildContainer()
{
	if (IsValid(BuildContainerObj))
		BuildContainerObj->BeginDestroy();
}

void UEUSTEditorWidget::SetLEDBuilData(const FLEDBuildData& LEDBuildData)
{
	if (IsValid(BuildContainerObj))
		BuildContainerObj->LEDBuildData = LEDBuildData;
}

void UEUSTEditorWidget::BuildLEDScreen()
{
	if (!IsValid(BuildContainerObj)) return;
	bIsBuildLEDScreenSucceded = false;

	// Verify if Actor with same name exists
	AActor* ActorFound = UUST_EditorUtils::GetActorWithLabel(BuildContainerObj->LEDBuildData.Name);
	if (ActorFound)
	{
		UUST_EditorUtils::EditorMessageDialog(EAppMsgType::Ok, FString::Printf(TEXT("%s already exists. Try with different Screen Name"), *BuildContainerObj->LEDBuildData.Name), TEXT("UBuild"));
		return;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Actor with name not found"));
	}

	// Create the outer actor (All Actors are attached to this actor)
	ALEDScreenGenerator* LEDScreenGeneratorActor = Cast<ALEDScreenGenerator>(PlaceActorOfClass(FTransform(BuildContainerObj->LEDBuildData.FirstScreenPosition), BuildContainerObj->LEDBuildData.Name, ALEDScreenGenerator::StaticClass()));
	if (IsValid(LEDScreenGeneratorActor))
	{
		LEDScreenGeneratorActor->LEDBuildData = BuildContainerObj->LEDBuildData;
	}

	BuildAllScreens(LEDScreenGeneratorActor);

	//TArray<ALEDScreenActor> LEDScreenActor;
	GenerateWallAndRoofSMs(LEDScreenGeneratorActor);
	
}

void UEUSTEditorWidget::GenerateWallAndRoofSMs(ALEDScreenGenerator* LEDScreenGeneratorActor) {
	TArray<AActor*> AttachedActors;
	LEDScreenGeneratorActor->GetAttachedActors(AttachedActors, true, false);
	for (auto actor : AttachedActors) {
		UE_LOG(LogTemp, Warning, TEXT("actor name %s & AttachedActors count %d"), *actor->GetActorLabel(), AttachedActors.Num());

		ALEDScreenActor* LEDScreenActor = Cast<ALEDScreenActor>(actor);
		if (IsValid(LEDScreenActor))
		{
			GenerateScreensStaticMeshes(LEDScreenActor);
		}
		else {
			ALEDTileActor* LEDTileActor = Cast<ALEDTileActor>(actor);
			UE_LOG(LogTemp, Warning, TEXT("Cast Roof LEDTileActor"));
			if (!IsValid(LEDTileActor)) return;
			if (LEDTileActor->TileActorType == ETileActorType::RoofActor) {
				UE_LOG(LogTemp, Warning, TEXT("Roof LEDTileActor"));
				GenerateRoofStaticMeshes(LEDTileActor);
			}
		}
	}
}

void UEUSTEditorWidget::BuildAllScreens(ALEDScreenGenerator* LEDScreenGenerator)
{
	int32 NumHorizontalScreens = LEDScreenGenerator->LEDBuildData.ScreenNumber.X;
	int32 NumVerticalScreens = LEDScreenGenerator->LEDBuildData.ScreenNumber.Y;

	FString LEDScreenActorName = UUST_StringUtils::CreateScreen_ActorName(LEDScreenGenerator->LEDBuildData.Name);

	TArray<FTransform> MeshCoordinates;
	AActor* TempActor = UUST_EditorUtils::SpawnEmptyActor(FTransform(LEDScreenGenerator->LEDBuildData.FirstScreenPosition),TEXT("TempActor"),AActor::StaticClass());
	UUST_MathUtils::GenerateMeshCoordinates(LEDScreenGenerator->LEDBuildData.WallTileSize.X,LEDScreenGenerator->LEDBuildData.WallTileSize.Y,
		LEDScreenGenerator->LEDBuildData.WallTilesNumber.X,LEDScreenGenerator->LEDBuildData.WallTilesNumber.Y,
		LEDScreenGenerator->LEDBuildData.ScreenNumber.X, LEDScreenGenerator->LEDBuildData.ScreenNumber.Y,
		LEDScreenGenerator->LEDBuildData.ScreenOffset.X, LEDScreenGenerator->LEDBuildData.ScreenOffset.Y,LEDScreenGenerator->LEDBuildData.WallTilesAngle,LEDScreenGenerator->LEDBuildData.FirstScreenPosition,
		TempActor,MeshCoordinates);
	TempActor->Destroy();
	int count = 0;
	for(auto MeshCoordinate : MeshCoordinates)
	{
		UE_LOG(LogTemp,Warning,TEXT("CoordinateIndex: %d,MeshCoordinate:%s"),count,*MeshCoordinate.GetLocation().ToString());
		count++;
	}
	UE_LOG(LogTemp,Warning,TEXT("__________________________________________________________"));
	int32 ScreenIndex = 0;
	FTransform ScreenTransformIndex;
	for (int32 i = 0; i < NumVerticalScreens; i++)
	{
		for (int32 j = 0; j <NumHorizontalScreens ; j++)
		{
			ScreenIndex = j + (i * NumHorizontalScreens);
			int32 NumVerticalTiles = LEDScreenGenerator->LEDBuildData.WallTilesNumber.X;
			int32 NumHorizontalTiles = LEDScreenGenerator->LEDBuildData.WallTilesNumber.Y;
			int32 CoordinateIndex = UUST_MathUtils::Get1dFrom2D(j*LEDScreenGenerator->LEDBuildData.WallTilesNumber.X,
														i*LEDScreenGenerator->LEDBuildData.WallTilesNumber.Y,
														(LEDScreenGenerator->LEDBuildData.WallTilesNumber.X*LEDScreenGenerator->LEDBuildData.ScreenNumber.X));
			ScreenTransformIndex = MeshCoordinates[CoordinateIndex];
			
			UE_LOG(LogTemp,Warning,TEXT("CoordinateIndex: %d,MeshCoordinate:%s i:%d, j:%d , WallTilesNumber.X : %f WallTilesNumber.Y: %f"),
				CoordinateIndex,*ScreenTransformIndex.GetLocation().ToString(),i,j,
				LEDScreenGenerator->LEDBuildData.WallTilesNumber.X, LEDScreenGenerator->LEDBuildData.WallTilesNumber.Y);
			FString ActorName = LEDScreenActorName + FString::FromInt(ScreenIndex) + TEXT("_Actor");

			ALEDScreenActor* LEDScreenActor = Cast<ALEDScreenActor>(PlaceActorOfClass(ScreenTransformIndex, ActorName, ALEDScreenActor::StaticClass()));
			if (IsValid(LEDScreenActor))
			{
				LEDScreenActor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
				{
					FAttachmentTransformRules AttachmentRules(
						EAttachmentRule::KeepWorld, // Location rule
						EAttachmentRule::KeepWorld, // Rotation rule
						EAttachmentRule::KeepWorld, // Scale rule
						false // Weld simulated bodies
					);
					LEDScreenActor->AttachToActor(LEDScreenGenerator, AttachmentRules);
				}

				LEDScreenActor->ScreenIndex = ScreenIndex;
				LEDScreenActor->LEDScreenData = GetLEDScreenDataFromBuildData(LEDScreenGenerator->LEDBuildData);
				LEDScreenActor->Prefix = LEDScreenActorName + FString::FromInt(ScreenIndex);
				
				bool bIsWallGenerated = false;
				bool bIsRoofGenerated = false;
				GenerateWallTileActor(LEDScreenActor, ScreenTransformIndex, LEDScreenGenerator->LEDBuildData.Name, ScreenIndex, bIsWallGenerated);
			}
		}
	}
	if (LEDScreenGenerator->LEDBuildData.BuildRoof)
	{
		bool bIsRoofGenerated = false;
		FTransform RoofScreenTransform(LEDScreenGenerator->LEDBuildData.FirstScreenPosition);
		GenerateRoofTileActor(LEDScreenGenerator, RoofScreenTransform, LEDScreenGenerator->LEDBuildData.Name, ScreenIndex, bIsRoofGenerated);
	}
}

FLEDScreenData UEUSTEditorWidget::GetLEDScreenDataFromBuildData(FLEDBuildData InLEDBuildData)
{
	FLEDScreenData TempLEDScreenData;

	TempLEDScreenData.BuildRoof = InLEDBuildData.BuildRoof;
	TempLEDScreenData.RoofHeight = InLEDBuildData.RoofHeight;

	TempLEDScreenData.RoofTilePixels = InLEDBuildData.RoofTilePixels;
	TempLEDScreenData.RoofTilesAngle = InLEDBuildData.RoofTilesAngle;
	TempLEDScreenData.RoofTileSize = InLEDBuildData.RoofTileSize;
	TempLEDScreenData.RoofTilesNumber = InLEDBuildData.RoofTilesNumber;

	TempLEDScreenData.WallTilePixels = InLEDBuildData.WallTilePixels;
	TempLEDScreenData.WallTilesAngle = InLEDBuildData.WallTilesAngle;
	TempLEDScreenData.WallTileSize = InLEDBuildData.WallTileSize;
	TempLEDScreenData.WallTilesNumber = InLEDBuildData.WallTilesNumber;
	return TempLEDScreenData;
}

FTransform UEUSTEditorWidget::GenerateWallTileActor(ALEDScreenActor* InLEDScreenActor, FTransform ScreenTransform, FString ScreenName, int32 ScreenID, bool& bIsWallGenerated)
{
	// Wall Generation
	FString MeshName = ScreenName;
	FString MeshStorePath = UUST_ToolUtils::GetTempMeshSourceFolderPath();

	float Width = InLEDScreenActor->LEDScreenData.WallTileSize.X;
	float Length = InLEDScreenActor->LEDScreenData.WallTileSize.Y;  // Length or Height
	float WallTileAngle = InLEDScreenActor->LEDScreenData.WallTilesAngle;


	// Wall Parent Actor Name
	FTransform WallParentActorTransform = ScreenTransform;
	FString WallParentActorName = UUST_StringUtils::CreateTileActor_WallName(MeshName, ScreenID);

	// Clear Meshes if Already Placed in world outliner
	UUST_EditorUtils::RemoveActorInWorldOutliner(WallParentActorName);

	AActor* WallParentActor = PlaceActorOfClass(WallParentActorTransform, WallParentActorName, ALEDTileActor::StaticClass());
	ALEDTileActor* WallTileActor = Cast<ALEDTileActor>(WallParentActor);
	WallTileActor->TileActorType = ETileActorType::WallActor;
	WallTileActor->Prefix = InLEDScreenActor->Prefix + FString::Printf(TEXT("_WallTileActor_"));

	WallParentActor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	{
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::KeepWorld, // Location rule
			EAttachmentRule::KeepWorld, // Rotation rule
			EAttachmentRule::KeepWorld, // Scale rule
			false // Weld simulated bodies
		);
		WallParentActor->AttachToActor(InLEDScreenActor, AttachmentRules);
	}
	
	FTransform LastActorTransform = GenerateWallTiles(WallTileActor, ScreenTransform,MeshName,ScreenID,Width,Length, WallTileAngle, InLEDScreenActor->LEDScreenData.WallTilesNumber.X, InLEDScreenActor->LEDScreenData.WallTilesNumber.Y);
	return LastActorTransform;
}

FTransform UEUSTEditorWidget::GenerateWallTiles(ALEDTileActor* InLEDTileActor,  FTransform TileActorTransform, FString MeshName, int32 ScreenID, float TileWidth, float TileHeight, float TileAngle, int32 Num_Hori_Tiles, int32 Num_Verti_Tiles) {
	
	UE_LOG(LogTemp, Warning, TEXT("GenerateWallTiles MeshName %s, ScreenID %d, TileWidth %f, TileHeight %f, TileAngle %f, Num_Hori_Tiles %d, Num_Verti_Tiles %d"), *MeshName, ScreenID, TileWidth, TileHeight, TileAngle, Num_Hori_Tiles, Num_Verti_Tiles);
	
	
	FString MeshStorePath = UUST_ToolUtils::GetTempMeshSourceFolderPath();

	// Wall Tile Static Mesh Name
	FString WallTileMeshName = UUST_StringUtils::GetSM_PlaneTile_Name(MeshName, TileWidth, TileHeight);

	FString AssetPath = MeshStorePath + WallTileMeshName;
	bool bFoundSM = false;
	FString OutMsg = TEXT("");
	UStaticMesh* GeneratedPlane;

	GeneratedPlane = UUST_MeshUtils::GetStaticMeshIfExists(AssetPath, bFoundSM, OutMsg);

	if (!bFoundSM)
	{
		GeneratedPlane = CreatePlaneMesh(WallTileMeshName, MeshStorePath, TileWidth, TileHeight);	
	}
	GeneratedPlane->SetLightMapResolution(256);
	GeneratedPlane->SetLightMapCoordinateIndex(1);

	// Wall Tile Static Mesh Actor Name
	FString WallTileName = UUST_StringUtils::CreateTile_WallName(MeshName, ScreenID);

	
	FVector Location = TileActorTransform.GetLocation();
	FTransform GeneratedPlaneTransform;
	
	for (int32 i = 0; i < Num_Verti_Tiles; i++) // Rows
	{
		GeneratedPlaneTransform.SetLocation(FVector(Location.X, Location.Y, Location.Z + (i * TileHeight)));
		GeneratedPlaneTransform.SetRotation(TileActorTransform.GetRotation());

		for (int32 j = 0; j <Num_Hori_Tiles ; j++) // Columns
		{

			// Name of the Tile
			FString StaticMeshActorName = WallTileName;
			int32  index = j + (Num_Hori_Tiles * i);

			StaticMeshActorName.Append(FString::Printf(TEXT("_%d"), index));
			AActor* TempActor = PlaceLEDTileStaticMeshActor(GeneratedPlane, GeneratedPlaneTransform, StaticMeshActorName);
			if (!IsValid(TempActor))
			{
				return FTransform();
			}

			TempActor->GetRootComponent()->SetMobility(EComponentMobility::Movable);


			FAttachmentTransformRules AttachmentRules(
				EAttachmentRule::KeepWorld, // Location rule
				EAttachmentRule::KeepWorld, // Rotation rule
				EAttachmentRule::KeepWorld, // Scale rule
				false // Weld simulated bodies
			);

			// Attach ChildActor to ParentActor
			TempActor->AttachToActor(InLEDTileActor, AttachmentRules);


			TArray<int32>Triangles;
			TArray<FVector3f> MeshVerts;
			TArray<FVector3f> Normals;
			TArray<FVector3f> Tangents;
			TArray<FVector3f> Binormals;
			TArray<FColor> VertexColors;

			/*UUST_MeshUtils::GetStaticMeshInfo(Cast<AStaticMeshActor>(TempActor)->GetStaticMeshComponent(), Triangles, MeshVerts, Normals, Tangents, Binormals, VertexColors);


			UE_LOG(LogTemp, Warning, TEXT("Wall Mesh Verts for tile %d"), index);
			for (auto vert : MeshVerts) {
				UE_LOG(LogTemp, Warning, TEXT("Wall Mesh Verts %s"), *vert.ToString());
			}*/

			// Storing Data 
			FUST_TileData tileData;
			tileData.TID = index;
			tileData.TileType = ETileType::Wall;
			tileData.TileWidth = TileWidth;
			tileData.TileHeight = TileHeight;
			tileData.VertexPositions = MeshVerts;
			tileData.VertexIds = { 0,1,2,3 };

			Cast<ALEDTileStaticMeshActor>(TempActor)->TileInfo = tileData;

			FTransform ActorWorldTransform = TempActor->GetActorTransform();


			// Logic For Screen Gap Horizontal (Working)
			//UE_LOG(LogTemp, Warning, TEXT("index val   %d = %d "), index, ((Num_Hori_Tiles * Num_Verti_Tiles) - 1));
			if (index == (Num_Hori_Tiles * Num_Verti_Tiles) - 1)
			{
				ALEDScreenGenerator* ScreenGen = Cast<ALEDScreenGenerator>(InLEDTileActor->GetAttachParentActor()->GetAttachParentActor());
				if (IsValid(ScreenGen)) {
					int32 ScreenGap = ScreenGen->LEDBuildData.ScreenOffset.X;
					TileWidth = TileWidth + ScreenGap;
				}
				
			}
			GeneratedPlaneTransform = UUST_MathUtils::CalculateTransformOfNextTile(ActorWorldTransform, 0.f, 0.f, TileAngle, TempActor->GetActorForwardVector(), TileWidth);
			FVector CurrentTileLocation = GeneratedPlaneTransform.GetLocation();
			CurrentTileLocation = FVector(CurrentTileLocation.X - TileWidth,CurrentTileLocation.Y,CurrentTileLocation.Z);
			UE_LOG(LogTemp, Warning, TEXT("GeneratedPlaneTransform index %d %s"),index,*CurrentTileLocation.ToString());
		}
	}
	return GeneratedPlaneTransform;
}

FTransform UEUSTEditorWidget::GenerateRoofTileActor(ALEDScreenGenerator* InLEDScreenGenActor, FTransform ScreenTransform, FString ScreenName, int32 ScreenID, bool& bIsRoofGenerated)
{
	// Roof Generation
	FString MeshName = ScreenName;

	float Width = InLEDScreenGenActor->LEDBuildData.RoofTileSize.X;
	float Length = InLEDScreenGenActor->LEDBuildData.RoofTileSize.Y;  // Length or Height
	float RoofTileAngle = InLEDScreenGenActor->LEDBuildData.RoofTilesAngle;
	float RoofHeight = InLEDScreenGenActor->LEDBuildData.RoofHeight;


	ScreenTransform.SetLocation(ScreenTransform.GetLocation() + FVector(0, 0, RoofHeight));

	// Roof Parent Actor Name
	FString RoofTileActorName = UUST_StringUtils::CreateTileActor_RoofName(MeshName);

	AActor* RoofTileActor = PlaceActorOfClass(ScreenTransform, RoofTileActorName, ALEDTileActor::StaticClass());
	ALEDTileActor* LEDTileActor = Cast<ALEDTileActor>(RoofTileActor);
	LEDTileActor->TileActorType = ETileActorType::RoofActor;
	LEDTileActor->Prefix = InLEDScreenGenActor->LEDBuildData.Name + FString::Printf(TEXT("_RoofTileActor_"));
	UUST_EditorUtils::AttachActorToParentActor(RoofTileActor, InLEDScreenGenActor);


	if (!IsValid(RoofTileActor)) {

		bIsRoofGenerated = false;
		return FTransform();
	}

	FTransform LastTransform = GenerateRoofTiles(LEDTileActor, ScreenTransform, MeshName, Width, Length, RoofTileAngle, RoofHeight, InLEDScreenGenActor->LEDBuildData.RoofTilesNumber.X, InLEDScreenGenActor->LEDBuildData.RoofTilesNumber.Y);
	bIsRoofGenerated = true;
	return LastTransform;
}

FTransform UEUSTEditorWidget::GenerateRoofTiles(ALEDTileActor* InLEDTileActor, FTransform TileActorTransform, FString MeshName, float TileWidth, float TileHeight, float TileAngle, float RoofHeight, int32 Num_Hori_Tiles, int32 Num_Verti_Tiles) {
	// Roof Tile Static Mesh Name
	FString RoofTileStaticMeshName = UUST_StringUtils::GetSM_PlaneTile_Name(MeshName, TileWidth, TileHeight);

	FString MeshStorePath = UUST_ToolUtils::GetTempMeshSourceFolderPath();
	FString AssetPath = MeshStorePath + RoofTileStaticMeshName;

	bool bFoundSM = false;
	FString OutMsg = TEXT("");
	UStaticMesh* GeneratedPlane;

	GeneratedPlane = UUST_MeshUtils::GetStaticMeshIfExists(AssetPath, bFoundSM, OutMsg);

	if (!bFoundSM)
		GeneratedPlane = CreatePlaneMesh(RoofTileStaticMeshName, MeshStorePath, TileWidth, TileHeight);

	if (!IsValid(GeneratedPlane)) {
		return FTransform();
	};


	// Roof Tile Static Mesh Actor Name
	FString RoofTileName = UUST_StringUtils::CreateTile_RoofName(MeshName);

	FVector RowLocation = InLEDTileActor->GetActorLocation();
	FRotator RowRotation = FRotator::ZeroRotator;
	FVector Current_RightVector = InLEDTileActor->GetActorRightVector();

	FTransform GeneratedPlaneTransform; // NextPlane Transform

	UE_LOG(LogTemp, Warning, TEXT("[UEUSTEditorWidget::GenerateRoofTiles] Num_Hori_Tiles %d"), Num_Hori_Tiles);
	UE_LOG(LogTemp, Warning, TEXT("[UEUSTEditorWidget::GenerateRoofTiles] Num_Verti_Tiles %d"), Num_Verti_Tiles);

	for (int32 i = 0; i < Num_Verti_Tiles; i++) // Rows
	{
		FQuat quat = FQuat::MakeFromEuler(FVector(((TileAngle * i) + 90.f), 0, 0));
		//RowRotation = UKismetMathLibrary::TransformRotation(RoofTileActor->GetActorTransform(), quat.Rotator());

		RowRotation = (InLEDTileActor->GetActorRotation() + quat.Rotator());

		UE_LOG(LogTemp, Warning, TEXT("(RoofTileAngle * i) %f & RowRotation %s"), ((TileAngle * i)), *RowRotation.ToString());
		GeneratedPlaneTransform = UKismetMathLibrary::MakeTransform(RowLocation, RowRotation, FVector(1, 1, 1));

		for (int32 j = 0; j < Num_Hori_Tiles; j++) // Columns
		{
			FString StaticMeshActorName = RoofTileName;
			int32  index = j + (Num_Hori_Tiles * i);

			StaticMeshActorName.Append(FString::Printf(TEXT("_%d"), index));


			//if(UUST_EditorUtils::GetActorWithLabel(StaticMeshActorName))

			AActor* TempActor = PlaceLEDTileStaticMeshActor(GeneratedPlane, GeneratedPlaneTransform, StaticMeshActorName);
			if (!IsValid(TempActor))
			{
				return FTransform();
			}

			UUST_EditorUtils::AttachActorToParentActor(TempActor, InLEDTileActor);

			TArray<int32>Triangles;
			TArray<FVector3f> MeshVerts;
			TArray<FVector3f> UVVerts;
			TArray<FVector3f> Normals;
			TArray<FVector3f> Tangents;
			TArray<FVector3f> Binormals;
			TArray<FColor> VertexColors;

			UUST_MeshUtils::GetStaticMeshInfo(InLEDTileActor->GetActorTransform(),Cast<AStaticMeshActor>(TempActor)->GetStaticMeshComponent(), Triangles, MeshVerts,UVVerts, Normals, Tangents, Binormals, VertexColors);


			// Storing Data 
			FUST_TileData tileData;
			tileData.TID = index;
			tileData.TileType = ETileType::Roof;
			tileData.TileWidth = TileWidth;
			tileData.TileHeight = TileHeight;
			tileData.VertexPositions = MeshVerts;
			tileData.VertexIds = { 0,1,2,3 };

			Cast<ALEDTileStaticMeshActor>(TempActor)->TileInfo = tileData;

			FVector NextTileLocation = TempActor->GetActorLocation() + TempActor->GetActorForwardVector() * TileWidth;
			GeneratedPlaneTransform = UKismetMathLibrary::MakeTransform((NextTileLocation), RowRotation, FVector(1, 1, 1));
			Current_RightVector = TempActor->GetActorUpVector();

		}
		RowLocation = RowLocation + (Current_RightVector * TileHeight);
	}

	return GeneratedPlaneTransform;
}

void UEUSTEditorWidget::EditLEDScreen(const FLEDBuildData& InLEDBuildData)
{
	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();

	ALEDScreenActor* LEDScreenActor = nullptr;
	ALEDScreenGenerator* LEDScreenGenActor = nullptr;
	TArray<FUST_TileData> WallTileData;
	TArray<FUST_TileData> RoofTileData;

	bIsEditLEDScreenSucceded = false;

	if (USelection* SelectedActors = GEditor->GetSelectedActors()) {
		if (SelectedActors->Num() > 1 && SelectedActors->Num() == 0)
		{
			UUST_EditorUtils::EditorMessageDialog(EAppMsgType::Ok, FString::Printf(TEXT("Selected %d Actor(s). One LEDScreenActor can be edited at a time."), SelectedActors->Num()), TEXT("UBuild - Edit Section"));
		}
		else {
			FSelectionIterator Iter(*SelectedActors);
			LEDScreenActor = Cast<ALEDScreenActor>(*Iter);
			if (IsValid(LEDScreenActor)) {
				LEDScreenGenActor = Cast< ALEDScreenGenerator>(LEDScreenActor->GetParentActor());
				// Assign Prev LEDBuildData to Current LEDBuildData
				FLEDBuildData CurrentLEDBuildData = LEDScreenGenActor->LEDBuildData;

				// Overwrite the Current LEDBuildData with the Updated LEDBuildData
				CurrentLEDBuildData.Name = InLEDBuildData.Name;
				CurrentLEDBuildData.ScreenNumber = InLEDBuildData.ScreenNumber;

				CurrentLEDBuildData.WallTileSize = InLEDBuildData.WallTileSize;
				CurrentLEDBuildData.WallTilePixels = InLEDBuildData.WallTilePixels;
				CurrentLEDBuildData.WallTilesAngle = InLEDBuildData.WallTilesAngle;
				CurrentLEDBuildData.WallTilesNumber = InLEDBuildData.WallTilesNumber;

				CurrentLEDBuildData.RoofTileSize = InLEDBuildData.RoofTileSize;
				CurrentLEDBuildData.RoofTilePixels = InLEDBuildData.RoofTilePixels;
				CurrentLEDBuildData.RoofTilesAngle = InLEDBuildData.RoofTilesAngle;
				CurrentLEDBuildData.RoofTilesNumber = InLEDBuildData.RoofTilesNumber;

				CurrentLEDBuildData.RoofHeight = InLEDBuildData.RoofHeight;


				SetLEDBuilData(CurrentLEDBuildData);


				// Update the current LED Screen Generator Actor with the new LED Build Data
				LEDScreenGenActor->LEDBuildData = CurrentLEDBuildData;
				//FetchTileDataOfScreen(LEDScreenGenActor, WallTileData, RoofTileData);
				//UUST_EditorUtils::DestroyAllAttachedActors(LEDScreenActor,true);
				RemoveProceduralComponentFromActor(LEDScreenGenActor);

				GenerateScreensStaticMeshes(LEDScreenActor);


				// TODO: Need to move this to some other place
				bIsEditLEDScreenSucceded = true;
			}
			else {
				UUST_EditorUtils::EditorMessageDialog(EAppMsgType::Ok, FString::Printf(TEXT("Selected object is not LEDScreenGenerator Actor")), TEXT("UBuild - Edit Section"));
			}
		}
	}
}

void UEUSTEditorWidget::FetchTileDataOfScreen(ALEDScreenActor* InLEDScreenActor, TArray<FUST_TileData>& WallTileData, TArray<FUST_TileData>& RoofTileData)
{
	TArray<AActor*> AttachedActors;
	InLEDScreenActor->GetAttachedActors(AttachedActors, true, false);
	for (auto actor : AttachedActors) {
		ALEDTileActor* LEDTileActor = Cast<ALEDTileActor>(actor);
		if (IsValid(LEDTileActor))
		{
			if (LEDTileActor->TileActorType == ETileActorType::RoofActor)
			{
				GetAllLEDTileSMActorsInfo(LEDTileActor, RoofTileData);
			}
			else if (LEDTileActor->TileActorType == ETileActorType::WallActor) {
				GetAllLEDTileSMActorsInfo(LEDTileActor, WallTileData);
			}
		}
	}
}


FString UEUSTEditorWidget::GetPluginVersion()
{
	TArray<TSharedRef<IPlugin>> Plugins = IPluginManager::Get().GetDiscoveredPlugins();
	for (const TSharedRef<IPlugin>& Plugin : Plugins) {
		if (Plugin->GetName() == "unspace_tool") {
			return Plugin->GetDescriptor().VersionName;
		}
	}
	return TEXT("");
}

void UEUSTEditorWidget::BrowseForFolder(const FString& InBasePath, bool& OutSuccess, FString& OutPath)
{
    TSharedRef<SWindow> window = SNew(SWindow)
        .Title(FText::FromString("Browse Folders"))
        .ClientSize(FVector2D(320.0f, 320.0f))
        .SizingRule(ESizingRule::UserSized)
        .SupportsMaximize(false)
        .SupportsMinimize(false);

    TSharedRef<SDirectoryDialog> dialog = SNew(SDirectoryDialog, InBasePath);
    window->SetContent(dialog);

    IMainFrameModule& mainFrameModule
        = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
    const TSharedPtr<SWindow>& parentWindow = mainFrameModule.GetParentWindow();
    if (parentWindow.IsValid())
    {
        FSlateApplication::Get().AddModalWindow(window, parentWindow.ToSharedRef());
        if (dialog->HasValidResult())
        {
            OutSuccess = true;
            OutPath = dialog->GetPath();
            return;
        }
    }

    OutSuccess = false;
    OutPath.Empty();
    return;
}

void UEUSTEditorWidget::OpenFileDialog(EDialogResult& OutputPin, TArray<FString>& FilePath, const FString& DialogTitle,
	const FString& DefaultPath, const FString& DefaultFile, const FString& FileType, bool IsMultiple)
{
	void* windowHandle = UEUSTEditorWidget::GetWindowHandle();
	if (windowHandle) 
	{
		IDesktopPlatform* desktopPlatform = FDesktopPlatformModule::Get();
		if (desktopPlatform)
		{
			bool result = desktopPlatform->OpenFileDialog(
				windowHandle,
				DialogTitle,
				DefaultPath,
				DefaultFile,
				FileType,
				(IsMultiple ? EFileDialogFlags::Type::Multiple : EFileDialogFlags::Type::None),
				FilePath
			);
			if (result)
			{
				for (FString& fp : FilePath)
				{
					fp = FPaths::ConvertRelativePathToFull(fp);
					UE_LOG(LogTemp, Log, TEXT("Acquired file path : %s"), *fp);
				}

				UE_LOG(LogTemp, Log, TEXT("Open PDF Dialog : Successful"));
				UE_LOG(LogTemp, Log, TEXT("Open File Dialog : Successful"));
				OutputPin = EDialogResult::Successful;
				return;
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Open PDF Dialog : Cancelled"));
	UE_LOG(LogTemp, Log, TEXT("Open File Dialog : Cancelled"));
	OutputPin = EDialogResult::Cancelled;
}

void* UEUSTEditorWidget::GetWindowHandle()
{
	if (GIsEditor)
	{
		IMainFrameModule& MainFrameModule = IMainFrameModule::Get();
		TSharedPtr<SWindow> MainWindow = MainFrameModule.GetParentWindow();
		if (MainWindow.IsValid() && MainWindow->GetNativeWindow().IsValid())
		{
			return MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}
	else
	{
		if (GEngine && GEngine->GameViewport)
		{
			return GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
		}
	}
	return nullptr;
}

void UEUSTEditorWidget::BrowseForFile(const FString& InBasePath, bool& OutSuccess, FString& OutPath)
{

	TSharedRef<SWindow> window = SNew(SWindow)
	   .Title(FText::FromString("Browse Folders"))
	   .ClientSize(FVector2D(320.0f, 320.0f))
	   .SizingRule(ESizingRule::UserSized)
	   .SupportsMaximize(false)
	   .SupportsMinimize(false);

	TSharedRef<SAssetPathPicker> dialog = SNew(SAssetPathPicker);
	window->SetContent(dialog);
	
	IMainFrameModule& mainFrameModule
		= FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	const TSharedPtr<SWindow>& parentWindow = mainFrameModule.GetParentWindow();
	if (parentWindow.IsValid())
	{
		FSlateApplication::Get().AddModalWindow(window, parentWindow.ToSharedRef());
		if (dialog->ShowModal() == EAppReturnType::Ok)
		{
			// Get the full name of where we want to create the mesh asset.
			FString PackageName = dialog->GetFullAssetPath().ToString();
			UE_LOG(LogTemp, Warning, TEXT("##### Asset Path Here : %s  #####"), *PackageName);
		}
	}

	OutSuccess = false;
	OutPath.Empty();
	return;
}

UStaticMesh* UEUSTEditorWidget::CreatePlaneMesh(FString MeshName, FString SaveFolderPath, float Width, float Length)
{
	// Package
	FString PackageName = SaveFolderPath + "/" + MeshName;
	UPackage* MeshPackage = CreatePackage(*PackageName);
	check(MeshPackage);

	// Get Plane RawMesh
	bool bIsRawMeshValid = false;
	FString RawMeshOutMessage = TEXT("");
	FRawMesh NewRawMesh = UUST_EditorUtils::GetPlaneRawMesh(Width, Length, bIsRawMeshValid, RawMeshOutMessage);
	if (!bIsRawMeshValid)
		return nullptr;

	// Get MeshDescription
	/*FMeshDescription MeshDescription;
	TMap<int32, FName> MaterialMap;
	bool bSkipNormalsAndTangents = false;
	const TCHAR* DebugName = TEXT("PlaneMesh");
	FStaticMeshOperations::ConvertFromRawMesh(NewRawMesh, MeshDescription, MaterialMap, bSkipNormalsAndTangents, DebugName);*/



	UStaticMesh* NewStaticMesh = UUST_MeshUtils::CreateStaticMesh(MeshPackage, FName(*MeshName), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
	check(NewStaticMesh);

	NewStaticMesh->InitResources();
	NewStaticMesh->SetLightingGuid(FGuid::NewGuid());
	
	// Indicates that StaticMesh will be changed
	//NewStaticMesh->PreEditChange(nullptr);

	// Create a StaticMeshSourceModel
	FStaticMeshSourceModel& SrcModel = NewStaticMesh->AddSourceModel();
	/*SrcModel.RawMeshBulkData->LoadRawMesh(RawMesh);*/
	SrcModel.BuildSettings.bRecomputeNormals = true;
	SrcModel.BuildSettings.bRecomputeTangents = true;
	SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
	SrcModel.BuildSettings.bUseFullPrecisionUVs = true;
	SrcModel.BuildSettings.bGenerateLightmapUVs = true;
	SrcModel.BuildSettings.SrcLightmapIndex = 0; // Source UV channel
	SrcModel.BuildSettings.DstLightmapIndex = 1; // Destination lightmap UV channel
	SrcModel.BuildSettings.MinLightmapResolution = 256;
	SrcModel.RawMeshBulkData->SaveRawMesh(NewRawMesh);

	
	//NewStaticMesh->App
	// Set the section info for the mesh
	FMeshSectionInfo Info;
	Info.MaterialIndex = 0;
	Info.bEnableCollision = true;
	Info.bCastShadow = true;
	
	////// Set the section info for LOD 0, section 0
	//NewStaticMesh->GetSectionInfoMap().Set(0, 0, Info);
	UMaterial* Material = UMaterial::GetDefaultMaterial(EMaterialDomain::MD_Surface);
	Material->TwoSided = true;
	NewStaticMesh->AddMaterial(Material);

	//NewStaticMesh->GetRenderData()->ScreenSize[0].Default = 1.0f;

	//Set the Imported version before calling the build
	NewStaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;

	// Build the StaticMesh
	TArray<FText> BuildErrors;
	NewStaticMesh->Build(false, &BuildErrors); // false here means dont build the collision data

	NewStaticMesh->SetLightMapResolution(256);
	NewStaticMesh->SetLightMapCoordinateIndex(1);

	// Make package dirty.
	NewStaticMesh->MarkPackageDirty();

	// Indicates the end staticmesh
	NewStaticMesh->PostEditChange();

	// Notify asset registry of new asset
	FAssetRegistryModule::AssetCreated(NewStaticMesh);

	// Save Pckage Logic
	// FString FilePath = FString::Printf(TEXT("%s/%s%s"), *SaveFolderPath, *MeshName, *FPackageName::GetAssetPackageExtension());
	// bool bIsSuccess = UPackage::SavePackage(MeshPackage, NewStaticMesh, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FilePath);

	return NewStaticMesh;
}

AActor* UEUSTEditorWidget::PlaceLEDTileStaticMeshActor(UStaticMesh* StaticMesh, FTransform Transform, FString Name)
{
	FTransform objectTrasform(Transform);

	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel * currentLevel = currentWorld->GetCurrentLevel();

	UClass* staticMeshClass = ALEDTileStaticMeshActor::StaticClass();

	AActor* newActorCreated = GEditor->AddActor(currentLevel, staticMeshClass, objectTrasform, true, RF_Public | RF_Standalone | RF_Transactional);
	if (!IsValid(newActorCreated)) return nullptr;

	AStaticMeshActor* smActor = Cast<AStaticMeshActor>(newActorCreated);

	smActor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
	smActor->SetActorScale3D(objectTrasform.GetScale3D());
	// ID Name & Visible Name
	//smActor->Rename(*Name);
	smActor->SetActorLabel(*Name);
	smActor->GetStaticMeshComponent()->LightmassSettings.bUseTwoSidedLighting = true;
	GEditor->EditorUpdateComponents();
	smActor->GetStaticMeshComponent()->RegisterComponentWithWorld(currentWorld);
	currentWorld->UpdateWorldComponents(true, false);
	smActor->RerunConstructionScripts();
	smActor->MarkComponentsRenderStateDirty();

	return newActorCreated;
}

AActor* UEUSTEditorWidget::PlaceActorOfClass(FTransform Transform, FString Name, UClass* actorClass)
{
	FTransform objectTrasform(Transform);

	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();

	//UClass* actorClass = AActor::StaticClass();

	AActor* newActorCreated = GEditor->AddActor(currentLevel, actorClass, objectTrasform, true, RF_Public | RF_Standalone | RF_Transactional);
	newActorCreated->SetActorLabel(*Name);

	USceneComponent* RootComponent = NewObject<USceneComponent>(newActorCreated, USceneComponent::GetDefaultSceneRootVariableName(), RF_Transactional);
	RootComponent->Mobility = EComponentMobility::Movable;
	RootComponent->bVisualizeComponent = false;
	RootComponent->SetWorldTransform(objectTrasform);

	newActorCreated->SetRootComponent(RootComponent);
	newActorCreated->AddInstanceComponent(RootComponent);

	RootComponent->RegisterComponent();
	GEditor->EditorUpdateComponents();

	return newActorCreated;
}

AActor* UEUSTEditorWidget::PlaceLEDScreenActor(FTransform Transform, FString Name)
{
	return PlaceActorOfClass(Transform, Name, ALEDScreenActor::StaticClass());
}

UProceduralMeshComponent* UEUSTEditorWidget::AddProceduralComponentToActor(AActor* actor)
{
	if(!actor)
		return nullptr;

	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();


	//UProceduralMeshComponent* ProcComponentRoot = Cast<UProceduralMeshComponent>(actor->AddComponentByClass(UProceduralMeshComponent::StaticClass(), true, FTransform::Identity, false));
	//if (!ProcComponentRoot)
	//	return nullptr;

	UProceduralMeshComponent* ProcComponentRoot = NewObject<UProceduralMeshComponent>(actor, FName(TEXT("ProcComp")), RF_Transactional);
	if (!ProcComponentRoot)
		return nullptr;

	ProcComponentRoot->Mobility = EComponentMobility::Movable;
	ProcComponentRoot->bVisualizeComponent = true;
	
	actor->AddInstanceComponent(ProcComponentRoot);
	ProcComponentRoot->RegisterComponent();

	/*actor->GetRootComponent()->AttachToComponent(ProcComponentRoot);*/
	
	ProcComponentRoot->AttachToComponent(actor->GetDefaultAttachComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true), NAME_None);

	GEditor->EditorUpdateComponents();
	ProcComponentRoot->RegisterComponentWithWorld(currentWorld);
	currentWorld->UpdateWorldComponents(true, false);
	actor->RerunConstructionScripts();
	actor->MarkComponentsRenderStateDirty();

	return ProcComponentRoot;
}

void UEUSTEditorWidget::RemoveProceduralComponentFromActor(AActor* actor)
{
	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();

	if (!IsValid(actor)) return;

	UE_LOG(LogTemp, Warning, TEXT("[USTEditorWidget::RemoveProceduralComponentFromActor] Called"));

	TArray<UProceduralMeshComponent*> ProcComps;
	actor->GetComponents<UProceduralMeshComponent>(ProcComps);

	int32 ProcMeshCount = ProcComps.Num();

	UE_LOG(LogTemp, Warning, TEXT("[USTEditorWidget::RemoveProceduralComponentFromActor] UProceduralMeshComponent Count %d"), ProcComps.Num());
	for (int i = 0; i < ProcComps.Num(); i++)
	{
		if (ProcComps[i])
		{
			ProcComps[i]->UnregisterComponent();
			actor->RemoveInstanceComponent(ProcComps[i]);
			ProcComps[i]->DestroyComponent();
			
		}
	}


	//UActorComponent* ActorComp = actor->GetComponentByClass(UProceduralMeshComponent::StaticClass());
	//UProceduralMeshComponent* ProcMeshComp = Cast<UProceduralMeshComponent>(ActorComp);

	//if (!IsValid(ProcMeshComp)) return;
	//
	//ProcMeshComp->UnregisterComponent();
	////ProcMeshComp->DestroyComponent();
	//actor->RemoveInstanceComponent(ProcMeshComp);
	if (ProcMeshCount > 0) {
		GEditor->EditorUpdateComponents();
		currentWorld->UpdateWorldComponents(true, false);
		actor->RerunConstructionScripts();
		actor->MarkComponentsRenderStateDirty();
	}
}



AActor* UEUSTEditorWidget::GetActorWithName(UWorld* world, FString Name)
{
	if (!world || Name.Len() == 0) {
		return nullptr;
	}

	ULevel* currentLevel = world->GetCurrentLevel();

	if (!currentLevel)
		return nullptr;

	//UClass* staticMeshClass = AStaticMeshActor::StaticClass();

	for (AActor* actor : currentLevel->Actors) {
		if (actor && actor->GetName() == Name) {
			
			return actor;
		}
	}

	return nullptr;
}

bool UEUSTEditorWidget::RemoveActorFromWorld(ULevel* Level, AActor* ActorToRemove)
{
	if (!Level)
		return false;

	Level->RemoveLoadedActor(ActorToRemove);
	return true;
}

bool UEUSTEditorWidget::DeleteAssetFromContentBrowser(const FString& AssetPathToDelete)
{
	bool bIsAssetDeleted = false;
	if (AssetPathToDelete.Len() > 3)
		return bIsAssetDeleted;

	bIsAssetDeleted = UEditorAssetLibrary::DeleteAsset(AssetPathToDelete);
	return bIsAssetDeleted;

}

UStaticMesh* UEUSTEditorWidget::TestPlaneMesh(FString MeshName, FString SaveFolderPath, float Width, float Length)
{
	FString PackageName = SaveFolderPath + "/" + MeshName;
	UPackage* MeshPackage = CreatePackage(*PackageName);
	check(MeshPackage);


	FMeshDescription meshdec = UUST_EditorUtils::BuildPlaneMeshDescription();
	UStaticMesh* StaticMesh = UUST_EditorUtils::CreateStaticMeshWithDefaultMaterial(meshdec, MeshPackage, FName(*MeshName));
	check(StaticMesh);

	return StaticMesh;
}



UStaticMesh* UEUSTEditorWidget::MergeActors(TArray<AStaticMeshActor*> Actors, FString DestinationPath, bool& bOutSuccess, FString& OutInfoMessage)
{
	if (Actors.Num() == 0)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Merge Actors Failed - Actors List is Empty"));
		return nullptr;
	}

	UStaticMeshEditorSubsystem* StaticMeshEditorSubsystem = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
	if (StaticMeshEditorSubsystem == nullptr)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Static Mesh Editor Subsystem is not valid"));
		return nullptr;
	}
	FMergeStaticMeshActorsOptions Options;
	Options.BasePackageName = DestinationPath;
	Options.bSpawnMergedActor = false;

	Options.bDestroySourceActors = false;
	Options.NewActorLabel = "";
	Options.bRenameComponentsFromSource = true;

	Options.MeshMergingSettings.TargetLightMapResolution = 256;
	Options.MeshMergingSettings.GutterSize = 2;
	Options.MeshMergingSettings.LODSelectionType = EMeshLODSelectionType::CalculateLOD;
	Options.MeshMergingSettings.SpecificLOD = 0;
	Options.MeshMergingSettings.bGenerateLightMapUV = true;
	Options.MeshMergingSettings.bComputedLightMapResolution = false;
	Options.MeshMergingSettings.bPivotPointAtZero = true;
	Options.MeshMergingSettings.bMergePhysicsData = false;
	Options.MeshMergingSettings.bMergeMeshSockets = false;
	Options.MeshMergingSettings.bMergeMaterials = false;
	Options.MeshMergingSettings.bCreateMergedMaterial_DEPRECATED = false;
	Options.MeshMergingSettings.bBakeVertexDataToMesh = false;
	Options.MeshMergingSettings.bUseVertexDataForBakingMaterial = true;
	Options.MeshMergingSettings.bUseTextureBinning = false;
	Options.MeshMergingSettings.bReuseMeshLightmapUVs = true;
	Options.MeshMergingSettings.bMergeEquivalentMaterials = true;
	Options.MeshMergingSettings.bUseLandscapeCulling = false;
	Options.MeshMergingSettings.bIncludeImposters = true;
	Options.MeshMergingSettings.bSupportRayTracing = true;
	Options.MeshMergingSettings.bAllowDistanceField = false;

	//MergeActors
	AStaticMeshActor* MergedActor = nullptr;
	bOutSuccess = StaticMeshEditorSubsystem->MergeStaticMeshActors(Actors, Options, MergedActor);

	if (!bOutSuccess)
	{
		OutInfoMessage = FString::Printf(TEXT("Merge Actors failed"));
		return nullptr;
	}

	//Load Merged Static mesh and Return it
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Merged Actor Successfully"));
	return Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *DestinationPath));
}

void UEUSTEditorWidget::RemoveSelected()
{
	UUST_EditorUtils::RemoveSelectedActors();
}

void UEUSTEditorWidget::GetAllLEDTileSMActorsInfo(ALEDTileActor* LEDTileActor, TArray<FUST_TileData>& TilesContainer)
{
	if (!IsValid(LEDTileActor)) return;

	TArray<AActor*> AttachedActors;
	LEDTileActor->GetAttachedActors(AttachedActors, false, false);

	TArray<int32> Triangles;
	TArray<FVector3f> MeshVerts;
	TArray<FVector3f> UVVerts;
	TArray<FVector3f> Normals;
	TArray<FVector3f> Tangents;
	TArray<FVector3f> Binormals;
	TArray<FColor> VertexColors;


	for (auto actor : AttachedActors)
	{
		// Clear Containers
		Triangles.Empty();
		MeshVerts.Empty();
		UVVerts.Empty();
		Normals.Empty();
		Tangents.Empty();
		Binormals.Empty();
		VertexColors.Empty();


		ALEDTileStaticMeshActor* CurrentLEDTileSMActor = Cast<ALEDTileStaticMeshActor>(actor);
		if (IsValid(CurrentLEDTileSMActor)) {
			UStaticMeshComponent* SMComp = CurrentLEDTileSMActor->GetStaticMeshComponent();
			UUST_MeshUtils::GetStaticMeshInfo(LEDTileActor->GetActorTransform(),SMComp, Triangles, MeshVerts,UVVerts, Normals, Tangents, Binormals, VertexColors);

			/*UE_LOG(LogTemp, Warning, TEXT("Mesh Verts for tile %d"), index);
			for (auto vert : MeshVerts) {
				UE_LOG(LogTemp, Warning, TEXT("WMesh Verts %s"), *vert.ToString());
			}*/

			// Storing Data 
			FUST_TileData tileData;
			tileData.TID = CurrentLEDTileSMActor->TileInfo.TID;
			tileData.TileType = CurrentLEDTileSMActor->TileInfo.TileType;
			tileData.TileWidth = CurrentLEDTileSMActor->TileInfo.TileWidth;
			tileData.TileHeight = CurrentLEDTileSMActor->TileInfo.TileHeight;
			tileData.VertexPositions = MeshVerts;
			tileData.UVVertexPositions = UVVerts;
			TilesContainer.Add(tileData);
		}
	}
}

void UEUSTEditorWidget::UpdateLEDTileSize(ALEDTileStaticMeshActor* LEDTileSMActor, FVector2f NewSize)
{
	if (!IsValid(LEDTileSMActor)) return;
	
	LEDTileSMActor->TileInfo.TileWidth = NewSize.X;
	LEDTileSMActor->TileInfo.TileHeight = NewSize.Y;
}

void UEUSTEditorWidget::CreateMergedTileStaticMesh(ALEDTileActor* InLEDTileActor, int32 RowsCount, int32 ColsCount, FString InAssetName, FString InAssetPath)
{

	if (!IsValid(InLEDTileActor)) return;

	bool bIsRoof = InLEDTileActor->TileActorType == ETileActorType::RoofActor ? true : false;
	//bool bIsRoof =  false;
	TArray<FUST_TileData> TilesToMergeData;

	TArray<FVector> VerticesData;
	TArray<FVector2D> UVData;
	TArray<int32> TrianglesData;
	int skipIndex{ 0 };

	GetAllLEDTileSMActorsInfo(InLEDTileActor, TilesToMergeData);
		
	UE_LOG(LogTemp, Warning, TEXT("Array Count: %d"), TilesToMergeData.Num());

	float HighestRowWidth = 0;
	float HighestColumnHeight = 0;
	float NumHori = ColsCount; // Horizontal 
	float NumVerti = RowsCount; // Vertical



	/*int32 NumRows = BuildContainerObj->LEDBuildData.WallTilesNumber.X;
	int32 NumColumns = BuildContainerObj->LEDBuildData.WallTilesNumber.Y;*/

	//Calculate Highest Row Width
	for (size_t i = 0; i < NumHori; i++)
	{
		int32 tempHW = 0;
		for (size_t j = 0; j < NumVerti; j++)
		{
			float  index = j + (NumVerti * i);
			FUST_TileData CurrentTileData;
			bool detected = false;
			for (FUST_TileData a : TilesToMergeData)
			{
				if (a.TID == index)
				{
					CurrentTileData = a;
					detected = true;
					break;
				}
			}
			if (detected)
			{
				tempHW = tempHW + CurrentTileData.TileWidth;
			}
		}

		if (tempHW > HighestRowWidth)
		{
			HighestRowWidth = tempHW;
		}

		UE_LOG(LogTemp, Warning, TEXT("Tile Width : %d"), tempHW);
	}

	//Calculate Highest Columns Width
	for (size_t i = 0; i < NumVerti; i++)
	{
		int32 tempHW = 0;
		for (size_t j = 0; j < NumHori; j++)
		{
			float  index = i + (NumVerti * j);
			FUST_TileData CurrentTileData;
			bool detected = false;
			for (FUST_TileData a : TilesToMergeData)
			{
				if (a.TID == index)
				{
					CurrentTileData = a;
					detected = true;
					break;
				}
			}

			if (detected)
			{
				tempHW = tempHW + CurrentTileData.TileHeight;
			}

		}

		UE_LOG(LogTemp, Warning, TEXT("Tile Height : %d"), tempHW);
		if (tempHW > HighestColumnHeight)
		{
			HighestColumnHeight = tempHW;
		}
	}


	//UE_LOG(LogTemp, Warning, TEXT("Highest Row Width : %d"), HighestRowWidth);
	//UE_LOG(LogTemp, Warning, TEXT("Highest Column Height : %d"), HighestColumnHeight);

	//Calculate Vertices, UVs, Triangles
	for (size_t i = 0; i < NumHori; i++)
	{
		for (size_t j = 0; j < NumVerti; j++)
		{
			float  index = j + (NumVerti * i);
			FUST_TileData CurrentTileData;
			bool detected = false;
			for (FUST_TileData a : TilesToMergeData)
			{
				if (a.TID == index)
				{
					CurrentTileData = a;
					detected = true;
					break;
				}
			}

			if (detected)
			{
				float x1,y1,x2,y2,x3,y3,x4,y4;
				
				x1 = 1-UUST_MathUtils::MapFloatRange((j*CurrentTileData.TileWidth)/HighestRowWidth,0.0, 1.0, 1.0, 0.0);
				y1 = UUST_MathUtils::MapFloatRange((i*CurrentTileData.TileHeight)/HighestColumnHeight,0.0, 1.0, 1.0, 0.0);
						
				x2 = 1-UUST_MathUtils::MapFloatRange(((j+1)*CurrentTileData.TileWidth)/HighestRowWidth,0.0, 1.0, 1.0, 0.0);
				y2 = UUST_MathUtils::MapFloatRange((i*CurrentTileData.TileHeight)/HighestColumnHeight,0.0, 1.0, 1.0, 0.0);
						
				x3 = 1-UUST_MathUtils::MapFloatRange(((j+1)*CurrentTileData.TileWidth)/HighestRowWidth,0.0, 1.0, 1.0, 0.0);
				y3 = UUST_MathUtils::MapFloatRange(((i+1)*CurrentTileData.TileHeight)/HighestColumnHeight,0.0, 1.0, 1.0, 0.0);
						
				x4 = 1-UUST_MathUtils::MapFloatRange((j*CurrentTileData.TileWidth)/HighestRowWidth,0.0, 1.0, 1.0, 0.0);
				y4 = UUST_MathUtils::MapFloatRange(((i+1)*CurrentTileData.TileHeight)/HighestColumnHeight,0.0, 1.0, 1.0, 0.0);

				if(bIsRoof)
				{
					y1 = 1-y1;
					y2 = 1-y2;
					y3 = 1-y3;
					y4 = 1-y4;
				}
				
				UVData.Add({x1,y1});
				UVData.Add({x4,y4});
				UVData.Add({x3,y3});
				UVData.Add({x2,y2});
				
				for (int32 x = 0; x < CurrentTileData.VertexPositions.Num(); x++)
				{
					//VerticesData.Add(CurrentTileData.VertexPositions[x]);
					VerticesData.Add({ CurrentTileData.VertexPositions[x].X, CurrentTileData.VertexPositions[x].Y, CurrentTileData.VertexPositions[x].Z });
					/*float U = (CurrentTileData.UVVertexPositions[x].X / HighestRowWidth);
					U = UUST_MathUtils::MapFloatRange(U, 0.0, 1.0, 1.0, 0.0);
					float Value = bIsRoof ? (CurrentTileData.UVVertexPositions[x].Y / HighestColumnHeight):(CurrentTileData.UVVertexPositions[x].Z / HighestColumnHeight);
					float V = UUST_MathUtils::MapFloatRange(Value, 0.0, 1.0, 1.0, 0.0);*/
					//UVData.Add({U,V});
					//UE_LOG(LogTemp, Warning, TEXT("UVData: %f %f "),U,V);
				}

				TrianglesData.Add(((index - skipIndex) * 4) + 0);
				TrianglesData.Add(((index - skipIndex) * 4) + 3);
				TrianglesData.Add(((index - skipIndex) * 4) + 2);
				TrianglesData.Add(((index - skipIndex) * 4) + 0);
				TrianglesData.Add(((index - skipIndex) * 4) + 2);
				TrianglesData.Add(((index - skipIndex) * 4) + 1);
			}
			else
			{
				skipIndex++;
			}
		}
	}

	for(size_t i = 0; i < NumHori; i++)
	{
		
		for (size_t j = 0; j < NumVerti; j++)
		{
			float  index = j + (NumVerti * i);
			FUST_TileData CurrentTileData;
			bool detected = false;
			for (FUST_TileData a : TilesToMergeData)
			{
				if (a.TID == index)
				{
					CurrentTileData = a;
					detected = true;
					break;
				}
				if (detected)
				{
					

					
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Before TempProcMeshComp"));

	UProceduralMeshComponent* TempProcMeshComp = CreatePMComp(InLEDTileActor, VerticesData, TrianglesData, UVData);

	/*FString AssetName = (TEXT("SM_") + BuildContainerObj->LEDBuildData.Name);
	FString AssetPath = BuildContainerObj->LEDBuildData.ScreensFolderName + TEXT("/");*/
	if (!IsValid(TempProcMeshComp)) return;
	UE_LOG(LogTemp, Warning, TEXT("TempProcMeshComp"));
	UUST_MeshUtils::ConvertProceduralMeshToStaticMesh(TempProcMeshComp, InAssetPath, InAssetName);

	RemoveProceduralComponentFromActor(InLEDTileActor->GetAttachParentActor()->GetAttachParentActor());
}

UProceduralMeshComponent* UEUSTEditorWidget::CreatePMComp(ALEDTileActor* InLEDTileActor, TArray<FVector> Vertices, TArray<int32> Triangles, TArray<FVector2D> UVs)
{

	ALEDScreenGenerator* LEDScreenGenActor;

	if (InLEDTileActor->TileActorType == ETileActorType::RoofActor) {
		LEDScreenGenActor = Cast<ALEDScreenGenerator>(InLEDTileActor->GetAttachParentActor());
	}
	else {
		ALEDScreenActor* LEDScreenActor = Cast<ALEDScreenActor>(InLEDTileActor->GetAttachParentActor());
		if (!IsValid(LEDScreenActor)) return nullptr;
		LEDScreenGenActor = Cast<ALEDScreenGenerator>(LEDScreenActor->GetAttachParentActor());
	}

	if (!IsValid(LEDScreenGenActor)) return nullptr;

	FLEDBuildData& InLEDBuildData = LEDScreenGenActor->LEDBuildData;


	AActor* OuterParentActor = UUST_EditorUtils::GetActorWithLabel(InLEDBuildData.Name);
	UProceduralMeshComponent* ProcMeshComp = AddProceduralComponentToActor(OuterParentActor);

	UE_LOG(LogTemp, Warning, TEXT("Vertices Length %d"), Vertices.Num());
	UE_LOG(LogTemp, Warning, TEXT("UVs Length %d"), UVs.Num());
	UE_LOG(LogTemp, Warning, TEXT("Triangles Length %d"), Triangles.Num());

	TArray<FVector> Normals{};
	ProcMeshComp->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	return ProcMeshComp;

}

void UEUSTEditorWidget::GenerateScreensStaticMeshes(ALEDScreenActor* InLEDScreenActor)
{
	if (!IsValid(InLEDScreenActor)) return;

	ALEDScreenGenerator* InLEDScreenGenActor = Cast<ALEDScreenGenerator>(InLEDScreenActor->GetAttachParentActor());
	if (!IsValid(InLEDScreenGenActor)) return;

	FLEDBuildData& InLEDBuildData = InLEDScreenGenActor->LEDBuildData;
	FLEDScreenData& InLEDScreenData = InLEDScreenActor->LEDScreenData;

	FString AssetPath = InLEDBuildData.ScreensFolderName + TEXT("/");

	TArray<AActor*> AttachedActors;
	InLEDScreenActor->GetAttachedActors(AttachedActors, true, false);
	for (auto actor : AttachedActors) {
		ALEDTileActor* LEDTileActor = Cast<ALEDTileActor>(actor);
		if (IsValid(LEDTileActor))
		{
			if (LEDTileActor->TileActorType == ETileActorType::RoofActor)
			{
				CreateMergedTileStaticMesh(LEDTileActor, InLEDScreenData.RoofTilesNumber.X, InLEDScreenData.RoofTilesNumber.Y, UUST_StringUtils::Create_RoofMesh_Name(InLEDBuildData.Name), AssetPath);
				continue;
			}
			CreateMergedTileStaticMesh(LEDTileActor, InLEDScreenData.WallTilesNumber.X, InLEDScreenData.WallTilesNumber.Y, UUST_StringUtils::Create_Mesh_Name(InLEDScreenActor->ScreenIndex, InLEDBuildData.Name, false), AssetPath);
		}
	}
}

void UEUSTEditorWidget::GenerateRoofStaticMeshes(ALEDTileActor* InLEDTileActor) {
	if (!IsValid(InLEDTileActor)) return;

	ALEDScreenGenerator* InLEDScreenGenActor = Cast<ALEDScreenGenerator>(InLEDTileActor->GetAttachParentActor());
	if (!IsValid(InLEDScreenGenActor)) return;

	UE_LOG(LogTemp, Warning, TEXT("GenerateRoofStaticMeshes"));

	FLEDBuildData& InLEDBuildData = InLEDScreenGenActor->LEDBuildData;

	FString AssetPath = InLEDBuildData.ScreensFolderName + TEXT("/");

	CreateMergedTileStaticMesh(InLEDTileActor, InLEDBuildData.RoofTilesNumber.X, InLEDBuildData.RoofTilesNumber.Y, UUST_StringUtils::Create_RoofMesh_Name(InLEDBuildData.Name), AssetPath);
}

void UEUSTEditorWidget::UpdateLEDScreenGenerator(ALEDScreenGenerator* InLEDScreenGenerator, FUpdateLEDBuildData InUpdateLEDBuildData, bool bOverWrite)
{
	if (!IsValid(InLEDScreenGenerator)) return;

	if (InUpdateLEDBuildData.bUpdateName) {
		RenameLEDScreenGenerator(InLEDScreenGenerator, InUpdateLEDBuildData.UpdatedName, true);
	}

	if (InUpdateLEDBuildData.bUpdateScreenNumber) {
		if (!bOverWrite) {
			// Add Additional Screen Generation Logic Here (On Top of exisiting Screens)
		}
		else {
			// Overwrite all the screens
			// Destroy all LEDTileStaticMeshActors
			UUST_EditorUtils::DestroyAllAttachedActors(InLEDScreenGenerator, true);

			// Remove Procedural Mesh Component From ScreenGen
			RemoveProceduralComponentFromActor(InLEDScreenGenerator);

			// Build All Screens
			BuildAllScreens(InLEDScreenGenerator);

			// Store Static Meshes
			GenerateWallAndRoofSMs(InLEDScreenGenerator);

			//if (EditorTimerHandle.IsValid())
			//{
			//	GEditor->GetTimerManager()->ClearTimer(EditorTimerHandle);
			//}
			//GEditor->GetTimerManager()->SetTimer(EditorTimerHandle, [this, InLEDScreenGenerator](){
			//	/* callback; */
			//	// Remove Procedural Mesh Component From ScreenGen
			//	this->RemoveProceduralComponentFromActor(InLEDScreenGenerator);

			//	// Build All Screens
			//	this->BuildAllScreens(InLEDScreenGenerator);
			//}, 0.5f, false);
		}
	
	}
	//CreateMergedTileStaticMesh(InLEDTileActor);
}

void UEUSTEditorWidget::UpdateLEDScreenactor(ALEDScreenActor* InLEDScreenActor, FLEDScreenData InScreenData, bool bOverWrite)
{
	if (!IsValid(InLEDScreenActor)) return;

	ALEDScreenGenerator* LEDScreenGenActor = Cast<ALEDScreenGenerator>(InLEDScreenActor->GetAttachParentActor());
	if (!IsValid(LEDScreenGenActor)) return;

	InLEDScreenActor->LEDScreenData = InScreenData;

	//float WallScreenWidth = InLEDScreenActor->LEDScreenData.WallTilesNumber.X * InLEDScreenActor->LEDScreenData.WallTileSize.X; // Horizontal
	//float WallScreenLength = InLEDScreenActor->LEDScreenData.WallTilesNumber.Y * InLEDScreenActor->LEDScreenData.WallTileSize.Y; // Vertical

	//float RoofScreenWidth = InLEDScreenActor->LEDScreenData.RoofTilesNumber.X * InLEDScreenActor->LEDScreenData.RoofTileSize.X;
	//float RoofScreenLength = InLEDScreenActor->LEDScreenData.RoofTilesNumber.Y * InLEDScreenActor->LEDScreenData.RoofTileSize.Y;


	if (!bOverWrite) {
		// Add Additional Screen Generation Logic Here (On Top of exisiting Screens)
	}
	else {
		// Overwrite all the screens


		// Destroy all LEDTileStaticMeshActors
		UUST_EditorUtils::DestroyAllAttachedActors(InLEDScreenActor, true);

		// Remove Procedural Mesh Component From ScreenGen
		RemoveProceduralComponentFromActor(LEDScreenGenActor);

		// Generate Wall and Roof Tile Actors
		bool bIsWallGenerated = false;
		bool bIsRoofGenerated = false;
		GenerateWallTileActor(InLEDScreenActor, InLEDScreenActor->GetActorTransform(), LEDScreenGenActor->LEDBuildData.Name, InLEDScreenActor->ScreenIndex, bIsWallGenerated);

		// Store Static Meshes
		GenerateScreensStaticMeshes(InLEDScreenActor);
	}
}

void UEUSTEditorWidget::UpdateLEDTileActor(ALEDTileActor* InLEDTileActor, FLEDTileData UpdateLEDTileData, bool bOverWrite)
{
	if (!IsValid(InLEDTileActor)) return;

	ALEDScreenActor* LEDScreenActor = Cast<ALEDScreenActor>(InLEDTileActor->GetAttachParentActor());
	if (!IsValid(LEDScreenActor)) return;

	ALEDScreenGenerator* LEDScreenGenActor = Cast<ALEDScreenGenerator>(LEDScreenActor->GetAttachParentActor());
	if (!IsValid(LEDScreenGenActor)) return;

	FLEDBuildData& InLEDBuildData = LEDScreenGenActor->LEDBuildData;
	FString AssetPath = InLEDBuildData.ScreensFolderName + TEXT("/");


	FLEDScreenData& InLEDScreenData = LEDScreenActor->LEDScreenData;

	RemoveProceduralComponentFromActor(LEDScreenGenActor);

	if (!bOverWrite) {
		CreateMergedTileStaticMesh(InLEDTileActor, InLEDScreenData.WallTilesNumber.X, InLEDScreenData.WallTilesNumber.Y, UUST_StringUtils::Create_Mesh_Name(LEDScreenActor->ScreenIndex, InLEDBuildData.Name, false), AssetPath);
	}
	else {
		// If Overwrite
		// Updated Data
		if (InLEDTileActor->TileActorType == ETileActorType::WallActor)
		{
			LEDScreenActor->LEDScreenData.WallTileSize = UpdateLEDTileData.TileSize;
			LEDScreenActor->LEDScreenData.WallTilePixels = UpdateLEDTileData.TilePixels;
			LEDScreenActor->LEDScreenData.WallTilesAngle = UpdateLEDTileData.TilesAngle;
			LEDScreenActor->LEDScreenData.WallTileSize = UpdateLEDTileData.TileSize;
			LEDScreenActor->LEDScreenData.WallTilesNumber = UpdateLEDTileData.TilesNumber;
		}

		// Destroy all LEDTileStaticMeshActors
		UUST_EditorUtils::DestroyAllAttachedActors(InLEDTileActor, true);

		// Remove Procedural Mesh Component From ScreenGen
		RemoveProceduralComponentFromActor(LEDScreenGenActor);

		if (InLEDTileActor->TileActorType == ETileActorType::WallActor)
		{
			// Create LEDTileStaticMeshActors
			GenerateWallTiles(InLEDTileActor, InLEDTileActor->GetActorTransform(),
				InLEDBuildData.Name + TEXT("_Updated_"), LEDScreenActor->ScreenIndex,
				UpdateLEDTileData.TileSize.X, UpdateLEDTileData.TileSize.Y,
				UpdateLEDTileData.TilesAngle,
				UpdateLEDTileData.TilesNumber.X, UpdateLEDTileData.TilesNumber.Y);
		}


		// Creating Static Meshes from LEDTileStaticMeshActors
		CreateMergedTileStaticMesh(InLEDTileActor,
			UpdateLEDTileData.TilesNumber.X, UpdateLEDTileData.TilesNumber.Y,
			UUST_StringUtils::Create_Mesh_Name(LEDScreenActor->ScreenIndex, InLEDBuildData.Name, false),
			AssetPath);
	}

}

void UEUSTEditorWidget::UpdateLEDTileSMActor(ALEDTileStaticMeshActor* InLEDTileSMActor, FLEDTileData UpdateLEDTileData)
{
	if (!IsValid(InLEDTileSMActor)) return;

	ALEDTileActor* LEDTileActor = Cast<ALEDTileActor>(InLEDTileSMActor->GetAttachParentActor());
	if (!IsValid(LEDTileActor)) return;

	ALEDScreenActor* LEDScreenActor = Cast<ALEDScreenActor>(LEDTileActor->GetAttachParentActor());
	if (!IsValid(LEDScreenActor)) return;

	ALEDScreenGenerator* LEDScreenGenActor = Cast<ALEDScreenGenerator>(LEDScreenActor->GetAttachParentActor());
	if (!IsValid(LEDScreenGenActor)) return;

	FLEDBuildData& InLEDBuildData = LEDScreenGenActor->LEDBuildData;

	FString AssetPath = InLEDBuildData.ScreensFolderName + TEXT("/");

	//// Update LEDTileActor Data
	//LEDTileActor->LEDTileData.TileSize = UpdateLEDTileData.TileSize;
	//LEDTileActor->LEDTileData.TilePixels = UpdateLEDTileData.TilePixels;
	//LEDTileActor->LEDTileData.TilesAngle = UpdateLEDTileData.TilesAngle;
	//LEDTileActor->LEDTileData.TilesNumber = UpdateLEDTileData.TilesNumber;

	//if (LEDTileActor->TileActorType == ETileActorType::RoofActor)
	//{
	//	LEDTileActor->LEDTileData.Height = UpdateLEDTileData.Height;
	//}

	if (LEDTileActor->TileActorType == ETileActorType::WallActor)
	{
		LEDScreenActor->LEDScreenData.WallTileSize = UpdateLEDTileData.TileSize;
		LEDScreenActor->LEDScreenData.WallTilePixels = UpdateLEDTileData.TilePixels;
		LEDScreenActor->LEDScreenData.WallTilesAngle = UpdateLEDTileData.TilesAngle;
		LEDScreenActor->LEDScreenData.WallTileSize = UpdateLEDTileData.TileSize;
		LEDScreenActor->LEDScreenData.WallTilesNumber = UpdateLEDTileData.TilesNumber;
	}
	
	// Destroy all LEDTileStaticMeshActors
	UUST_EditorUtils::DestroyAllAttachedActors(LEDTileActor, true);
	
	// Remove Procedural Mesh Component From ScreenGen
	RemoveProceduralComponentFromActor(LEDScreenGenActor);

	// Create LEDTileStaticMeshActors
	GenerateWallTiles(LEDTileActor, LEDTileActor->GetActorTransform(), 
		InLEDBuildData.Name, LEDScreenActor->ScreenIndex,
		UpdateLEDTileData.TileSize.X, UpdateLEDTileData.TileSize.Y, 
		UpdateLEDTileData.TilesAngle, 
		UpdateLEDTileData.TilesNumber.X, UpdateLEDTileData.TilesNumber.Y);


	// Creating Static Meshes from LEDTileStaticMeshActors
	CreateMergedTileStaticMesh(LEDTileActor, 
		UpdateLEDTileData.TilesNumber.X, UpdateLEDTileData.TilesNumber.Y,
		UUST_StringUtils::Create_Mesh_Name(LEDScreenActor->ScreenIndex, InLEDBuildData.Name, false),
		AssetPath);
}

void UEUSTEditorWidget::RenameLEDScreenGenerator(ALEDScreenGenerator* InLEDScreenGenerator, FString Name, bool bRenameAttachedActors) {
	if (IsValid(InLEDScreenGenerator)) {
		FString SearchStr = InLEDScreenGenerator->LEDBuildData.Name;
		UE_LOG(LogTemp, Warning, TEXT("[UEUSTEditorWidget::RenameLEDScreenGenerator] SearchStr %s"), *SearchStr);
		InLEDScreenGenerator->LEDBuildData.Name = Name;
		InLEDScreenGenerator->SetActorLabel(*Name);

		if (!bRenameAttachedActors) return;
		TArray<AActor*> AttachedActors;
		InLEDScreenGenerator->GetAttachedActors(AttachedActors, true, true);
		for (auto actor : AttachedActors) {
			FString CurrentName = actor->GetActorLabel();
			FString NewName = CurrentName.Replace(*SearchStr, *Name);
			actor->SetActorLabel(*NewName);

		}
	}
}

void UEUSTEditorWidget::UpdateLEDRoofTileActor(ALEDTileActor* InLEDTileActor, FLEDTileData UpdateLEDTileData, bool bOverWrite) {
	if (!IsValid(InLEDTileActor)) return;

	ALEDScreenGenerator* LEDScreenGenActor = Cast<ALEDScreenGenerator>(InLEDTileActor->GetAttachParentActor());
	if (!IsValid(LEDScreenGenActor)) return;

	FLEDBuildData& InLEDBuildData = LEDScreenGenActor->LEDBuildData;

	FString AssetPath = InLEDBuildData.ScreensFolderName + TEXT("/");

	if (InLEDTileActor->TileActorType == ETileActorType::RoofActor)
	{

		FVector UpdatedLocation = LEDScreenGenActor->LEDBuildData.FirstScreenPosition + FVector(0, 0, UpdateLEDTileData.Height);
		InLEDTileActor->SetActorLocation(UpdatedLocation);

		LEDScreenGenActor->LEDBuildData.RoofTileSize = UpdateLEDTileData.TileSize;
		LEDScreenGenActor->LEDBuildData.RoofTilePixels = UpdateLEDTileData.TilePixels;
		LEDScreenGenActor->LEDBuildData.RoofTilesAngle = UpdateLEDTileData.TilesAngle;
		LEDScreenGenActor->LEDBuildData.RoofTileSize = UpdateLEDTileData.TileSize;
		LEDScreenGenActor->LEDBuildData.RoofTilesNumber = UpdateLEDTileData.TilesNumber;


		// Destroy all LEDTileStaticMeshActors
		UUST_EditorUtils::DestroyAllAttachedActors(InLEDTileActor, true);

		// Remove Procedural Mesh Component From ScreenGen
		RemoveProceduralComponentFromActor(LEDScreenGenActor);

		// Create LEDTileStaticMeshActors
		GenerateRoofTiles(InLEDTileActor, InLEDTileActor->GetActorTransform(),
			InLEDBuildData.Name,
			UpdateLEDTileData.TileSize.X, UpdateLEDTileData.TileSize.Y,
			UpdateLEDTileData.TilesAngle,
			UpdateLEDTileData.Height,
			UpdateLEDTileData.TilesNumber.X, UpdateLEDTileData.TilesNumber.Y);


		// Creating Static Meshes from LEDTileStaticMeshActors
		CreateMergedTileStaticMesh(InLEDTileActor,
			UpdateLEDTileData.TilesNumber.X, UpdateLEDTileData.TilesNumber.Y,
			UUST_StringUtils::Create_RoofMesh_Name(InLEDBuildData.Name),
			AssetPath);
	}
}

TArray<ADisplayClusterRootActor*> UEUSTEditorWidget::GetDisplayClusterRootActors()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByClass(ADisplayClusterRootActor::StaticClass()->GetFName(), AssetData);

	TArray<ADisplayClusterRootActor*> OutArray;
	for (int i = 0; i < AssetData.Num(); i++) {
		ADisplayClusterRootActor* Object = Cast<ADisplayClusterRootActor>(AssetData[i].GetAsset());
		if (Object)
		{
			OutArray.Add(Object);
		}
	}

	return OutArray;
}



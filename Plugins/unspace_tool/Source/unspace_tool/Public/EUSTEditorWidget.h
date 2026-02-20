// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BuildContainer.h"
#include "Actors/LEDTileActor.h"
#include "Modules/UST_AuthModule.h"
#include "Objects/UST_EditorContext_LevelEditor.h"
#include "Modules/UST_ConfigModule.h"
#include "Modules/UST_MonitorModule.h"
#include "EUSTEditorWidget.generated.h"

struct FRawMesh;
class UStaticMesh;
class AStaticMeshActor;
class UProceduralMeshComponent;


/** Called when a path is selected in the path picker */
DECLARE_DELEGATE_OneParam(FOnPathSelected, const FString& /*Path*/);


/**
 * 
 */
UENUM()
enum class EDialogResult : uint8
{
	Successful, Cancelled
};

UCLASS()
class UNSPACE_TOOL_API UEUSTEditorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()


	UBuildContainer* BuildContainerObj = nullptr;

	UPROPERTY()
	FTimerHandle EditorTimerHandle;

	UEUSTEditorWidget();

	~UEUSTEditorWidget();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST")
	UUST_EditorContext_LevelEditor* LevelEditor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UST|Build")
	bool bIsBuildLEDScreenSucceded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST|Build")
	bool bIsEditLEDScreenSucceded = false;

	
	
public:
	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void Setup();

	#pragma region AuthModule
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST|Auth")
		UUST_AuthModule* UST_AuthObj;
		UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Auth")
		UUST_AuthModule* CreateAuthModule();
	#pragma endregion
	
	#pragma region ConfigModule
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST|Config")
		UUST_ConfigModule* UST_ConfigObj;
	
		/* Config Module*/
		UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Config")
		void CreateConfigModule();

		UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Config")
		void CreatenDisplayConfigAsset(FnConfigAssetSettings AssetSettings);

		UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Config")
		void PlacenDisplayConfigAsset(FString ConfigPath,int32 TrackingActorCount, FnConfigCineCameraSettings CineCameraSettings , FnConfigFrustumSettings FrustumSettings);

		UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Config")
		TArray<FString> GetTrackingSystemNames();

		UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Config")
		void CreateOptiTrackSourceInLiveLinkPanel(FString InServerIPAddress, FString InClientIPAddress);

		UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Config")
		void CreateFreeDTrackSourceInLiveLinkPanel(FString InIPAddress, FString UDPPort);
	#pragma endregion

#pragma region MonitorModule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UST|Monitor")
	UUST_MonitorModule* UST_MonitorObj;
	
	/* Config Module*/
	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Monitor")
	void CreateMonitorModule();

	UFUNCTION(BlueprintCallable, Category= "UnSpaceTool | Monitor")
	void UpdateCineCameraSettings(ACineCameraActor* CineCameraActor,FnConfigCineCameraSettings CineCameraSettings);

	UFUNCTION(BlueprintCallable, Category= "UnSpaceTool | Monitor")
	void UpdateUConfigSettings(ADisplayClusterRootActor* nDisplayActor,FnConfigMonitorSettings MonitorSettings);

	UFUNCTION(BlueprintCallable, Category= "UnSpaceTool | Monitor")
	FnConfigCineCameraSettings FetchCineCameraSettings(ACineCameraActor* CineCameraActor);
	UFUNCTION(BlueprintCallable, Category= "UnSpaceTool | Monitor")
	FnConfigMonitorSettings FetchConfigMonitorSettings(ADisplayClusterRootActor* nConfigActor);
	
#pragma endregion
	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void CreateBuildContainer();

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void DeleteBuildContainer();

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void SetLEDBuilData(const FLEDBuildData& LEDBuildData);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void BuildLEDScreen();

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void BuildAllScreens(ALEDScreenGenerator* LEDScreenGenerator);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void GenerateWallAndRoofSMs(ALEDScreenGenerator* LEDScreenGeneratorActor);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	FLEDScreenData GetLEDScreenDataFromBuildData(FLEDBuildData LEDBuildData);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	FTransform GenerateWallTileActor(ALEDScreenActor* InLEDScreenActor, FTransform ScreenTransform, FString ScreenName, int32 ScreenID, bool& bIsWallGenerated);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	FTransform GenerateWallTiles(ALEDTileActor* InLEDTileActor, FTransform TileActorTransform, FString MeshName, int32 ScreenID, float TileWidth, float TileHeight, float TileAngle, int32 Num_Hori_Tiles, int32 Num_Verti_Tiles);


	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	FTransform GenerateRoofTileActor(ALEDScreenGenerator* InLEDScreenGenActor, FTransform ScreenTransform, FString ScreenName, int32 ScreenID, bool& bIsRoofGenerated);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	FTransform GenerateRoofTiles(ALEDTileActor* InLEDTileActor, FTransform TileActorTransform, FString MeshName, float TileWidth, float TileHeight, float TileAngle, float RoofHeight, int32 Num_Hori_Tiles, int32 Num_Verti_Tiles);


	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void EditLEDScreen(const FLEDBuildData& InLEDBuildData);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void FetchTileDataOfScreen(ALEDScreenActor* InLEDScreenActor, TArray<FUST_TileData>& WallTileData, TArray<FUST_TileData>& RoofTileData);

	/**
	 * Returns a version of the plugin.
	 */
	UFUNCTION(BlueprintPure, Category = "UnSpaceTool")
	FString GetPluginVersion();

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void BrowseForFolder(const FString& InBasePath, bool& OutSuccess, FString& OutPath);

	UFUNCTION(BlueprintCallable, Category = "QSUtility | OpenFileDialog", meta = (ExpandEnumAsExecs = "OutputPin"))
	static void OpenFileDialog(
		EDialogResult& OutputPin,
		TArray<FString>& FilePath,
		const FString& DialogTitle = TEXT("Open File Dialog"),
		const FString& DefaultPath = TEXT(""),
		const FString& DefaultFile = TEXT(""),
		const FString& FileType = TEXT("All (*)|*.*"),
		bool IsMultiple = false
	);
	static void* GetWindowHandle();

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void BrowseForFile(const FString& InBasePath, bool& OutSuccess, FString& OutPath);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	UStaticMesh* CreatePlaneMesh(FString MeshName = TEXT("TestSM"), FString SaveFolderPath = TEXT("/Game"), float Width = 100.f, float Length = 100.f);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	AActor* PlaceLEDTileStaticMeshActor(UStaticMesh* StaticMesh, FTransform Transform, FString Name);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	AActor* PlaceActorOfClass(FTransform Transform, FString Name, UClass* actorClass);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	AActor* PlaceLEDScreenActor(FTransform Transform, FString Name);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	UProceduralMeshComponent* AddProceduralComponentToActor(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void RemoveProceduralComponentFromActor(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	AActor* GetActorWithName(UWorld* world, FString Name);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	bool RemoveActorFromWorld(ULevel* Level, AActor* ActorToRemove);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	bool DeleteAssetFromContentBrowser(const FString& AssetPathToDelete);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	UStaticMesh* TestPlaneMesh(FString MeshName = TEXT("TestSM"), FString SaveFolderPath = TEXT("/Game"), float Width = 100.f, float Length = 100.f);


	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	static UStaticMesh* MergeActors(TArray<AStaticMeshActor*> Actors, FString DestinationPath, bool& bOutSuccess, FString& OutInfoMessage);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	static void RemoveSelected();

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	static void GetAllLEDTileSMActorsInfo(ALEDTileActor* LEDTileActor, TArray<FUST_TileData>& TilesContainer);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	static void UpdateLEDTileSize(ALEDTileStaticMeshActor* LEDTileSMActor, FVector2f NewSize);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void CreateMergedTileStaticMesh(ALEDTileActor* InLEDTileActor,int32 RowsCount, int32 ColsCount, FString InAssetName, FString InAssetPath);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	UProceduralMeshComponent* CreatePMComp(ALEDTileActor* InLEDTileActor, TArray<FVector> Vertices, TArray<int32> Triangles, TArray<FVector2D> UVs);


	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void GenerateScreensStaticMeshes(ALEDScreenActor* InLEDScreenActor); // Wall

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void GenerateRoofStaticMeshes(ALEDTileActor* InLEDTileActor);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void UpdateLEDScreenGenerator(ALEDScreenGenerator* InLEDScreenGenerator, FUpdateLEDBuildData InUpdateLEDBuildData, bool bOverWrite = false);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void UpdateLEDScreenactor(ALEDScreenActor* InLEDScreenActor, FLEDScreenData InScreenData, bool bOverWrite = false);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void UpdateLEDTileActor(ALEDTileActor* InLEDTileActor, FLEDTileData UpdateLEDTileData, bool bOverWrite = false);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void UpdateLEDTileSMActor(ALEDTileStaticMeshActor* InLEDTileSMActor, FLEDTileData UpdateLEDTileData);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void RenameLEDScreenGenerator(ALEDScreenGenerator* InLEDScreenGenerator, FString Name, bool bRenameAttachedActors = true);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	void UpdateLEDRoofTileActor(ALEDTileActor* InLEDTileActor, FLEDTileData UpdateLEDTileData, bool bOverWrite = false);


	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	TArray<ADisplayClusterRootActor*> GetDisplayClusterRootActors();
};
	


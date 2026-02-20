// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "UST_EditorUtils.generated.h"

struct FRawMesh;
struct FMeshDescription;

class UMaterial;
/**
/**
 * 
 */
UCLASS()
class UNSPACE_TOOL_API UUST_EditorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UST|Asset")
	static void OpenAssetWindow(UObject* Asset, bool& bOutSuccess, FString& OutInfoMsg);
	UFUNCTION(BlueprintCallable, Category = "UST|Asset")
	static void CloseAssetWindow(UObject* Asset, bool& bOutSuccess, FString& OutInfoMsg);
	UFUNCTION(BlueprintCallable, Category = "UST|Asset")
	static TArray<UObject*> GetAllAssetsWithOpenedWindow(bool& bOutSuccess, FString& OutInfoMsg);


	UFUNCTION(BlueprintCallable, Category = "UST|Asset")
	static UObject* CreateAsset(FString AssetPath, UClass* AssetClass, class UFactory* AssetFactory, bool& bOutSuccess, FString& OutInfoMsg);

	UFUNCTION(BlueprintCallable, Category = "UST|Asset")
	UMaterial* CreateMaterialAsset(FString AssetPath, bool& bOutSuccess, FString& OutInfoMsg);



	static FRawMesh GetPlaneRawMesh(float Width, float Length, bool& bOutSuccess, FString& OutInfoMsg);


	static FMeshDescription BuildPlaneMeshDescription();

	static FMeshDescription BuildGridMeshDescription(int x_count, int y_count, float x_scale, float y_scale);

	static UStaticMesh* CreateStaticMeshWithDefaultMaterial(FMeshDescription& Desc, UObject* InOuter, FName InName);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static void ShowEditorNotification(FString Message);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static void RemoveAssetAtPath(FString AssetPath, FString AssetToDelete);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static bool RemoveActorInWorldOutliner(FString Name);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static void RemoveSelectedActors();

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static AActor* GetActorWithLabel(FString Name);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static EAppReturnType::Type EditorMessageDialog(EAppMsgType::Type MessageType, const FString Message, const FString OptTitle);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static void SelectActorInWorldOutliner(AActor* ActorToSelect);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static void DestroyAllAttachedActors(AActor* ParentActor, bool bRecursiveActors = true);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static void CreateFolder(FString FolderPath, bool& bOutSuccess, FString& OutInfoMsg);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static void DeleteFolder(FString FolderPath, bool& bOutSuccess, FString& OutInfoMsg);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static void IsFolderExists(FString FolderPath, bool& bOutSuccess, FString& OutInfoMsg);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static void RenameActor(AActor* ActorToRename, FString NewName);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static void AttachActorToParentActor(AActor* Actor, AActor* ParentActor);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static AActor* SpawnEmptyActor(FTransform Transform, FString Name, UClass* actorClass);

	UFUNCTION(BlueprintCallable, Category = "UST|Editor")
	static AActor* SpawnActorFromClass(FTransform Transform, FString Name, UClass* actorClass);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	static UActorComponent* AddComponentToActor(AActor* actor, TSubclassOf<UActorComponent> ComponentClass , FString ComponentName);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	static void DeleteFilesInFolder(const FString& FolderPath);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	static bool DeleteDirectoryRecursively(const FString& DirectoryPath);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	static void LaunchProcess(FString FilePath,bool LaunchDetached);

};

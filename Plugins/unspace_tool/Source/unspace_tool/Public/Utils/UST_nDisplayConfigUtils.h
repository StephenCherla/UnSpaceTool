// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprints/DisplayClusterBlueprint.h"
#include "Components/DisplayClusterICVFXCameraComponent.h"
#include "Components/DisplayClusterXformComponent.h"
#include "Components/DisplayClusterScreenComponent.h"
#include "OpenColorIOConfiguration.h"
#include "DataClasses/nConfigSpawnData.h"
#include "Enums/UST_Types.h"
//#include "DisplayClusterConfigurationTypes.h"
#include "UST_nDisplayConfigUtils.generated.h"

/**
 * 
 */



UCLASS()
class UNSPACE_TOOL_API UUST_nDisplayConfigUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static UDisplayClusterBlueprint* CreateDisplayClusterBlueprintActor(FnConfigAssetSettings AssetSettings);
	
	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static UActorComponent* AddComponentToBlueprint(UDisplayClusterBlueprint* NewBlueprint, UClass* NewComponentClass, FName VariableName, USCS_Node* &OutNode);

	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static UActorComponent* GetFirstComponentByClass(UDisplayClusterBlueprint* NewBlueprint, UClass* NewComponentClass);
	
	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static void Remove_nDisplayScreenComponent(UDisplayClusterBlueprint* NewBlueprint);

	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static void set_nDisplayDefaultViewpointLocation(UDisplayClusterBlueprint* NewBlueprint , FVector DefaultLocation);

	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static void set_uDisplayTransformRotation(UDisplayClusterBlueprint* NewBlueprint, FQuat& quaterion);

	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static UDisplayClusterICVFXCameraComponent* CreateICVFXCameraComponent();

	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static UDisplayClusterConfigurationData* GetConfigData(UDisplayClusterBlueprint* ClusterAssetBlueprint);

	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static UDisplayClusterConfigurationCluster* GetCluster(UDisplayClusterConfigurationData* ClusterConfigData);

	/*UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static UDisplayClusterConfigurationHostDisplayData* AddHostDisplayDataToCluster(UBlueprint* Blueprint, UDisplayClusterConfigurationCluster* RootCluster, FString HostIPAddress, bool bCallPostEditChange);*/

	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static UDisplayClusterConfigurationClusterNode* AddClusterNodeToCluster(UBlueprint* Blueprint, UDisplayClusterConfigurationCluster* RootCluster, FString Name, FString Host,FVector2D WindowsResolution, bool bCallPostEditChange);

	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static UDisplayClusterConfigurationViewport* AddViewportToClusterNode(UBlueprint* Blueprint, UDisplayClusterConfigurationClusterNode* Node, FString Name, FVector2D ViewportResolution, bool bCallPostEditChange);

	UFUNCTION(BlueprintCallable, Category = "UST|nDisplayConfig")
	static void SelectSyncType(UBlueprint* Blueprint, UDisplayClusterConfigurationCluster* RootCluster,EClusterSyncType SyncType);

	static void CompileBlueprint(UBlueprint* NewBlueprint);
};

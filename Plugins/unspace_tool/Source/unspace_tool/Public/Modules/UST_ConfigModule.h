// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DisplayClusterRootActor.h"
#include "LiveLinkSourceFactory.h"
#include "Blueprints/DisplayClusterBlueprint.h"
#include "Components/DisplayClusterICVFXCameraComponent.h"
#include "Actors/nDisplayConfigActor.h"
#include "DataClasses/nConfigSpawnData.h"
#include "DataClasses/UMonitorLiveLinkData.h"
#include "UST_ConfigModule.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionUpdated, FLiveLinkDataToDisplay , Data);


class ACineCameraActor;
UCLASS()
class UNSPACE_TOOL_API UUST_ConfigModule : public UObject
{
	GENERATED_BODY()
	
public:
	UUST_ConfigModule();

	UDisplayClusterBlueprint* DisplayClustorBlueprintActor;

	UDisplayClusterConfigurationData* AssetClusterConfig;

	TObjectPtr<UDisplayClusterICVFXCameraComponent> ICVFXComponent;

	void CreateAndSave_nDisplayRootActorAsset(FnConfigAssetSettings AssetSettings);
	void PlaceConfigInEditorWorld(FString ConfigPath,int32 TrackingActorCount, FnConfigCineCameraSettings CineCameraSettings , FnConfigFrustumSettings FrustumSettings);
	void UpdateCineCameraSettings(ACineCameraActor* CineCameraActor, FnConfigCineCameraSettings CineCameraSettings);
	void UpdateFrustumValues(AActor* nConfigActor , FnConfigFrustumSettings FrustumSettings);

	void CreateOptiTrackSourceInLiveLinkPanel(FString InServerIPAddress,FString InClientIPAddress);
	void CreateFreeDTrackSourceInLiveLinkPanel(FString InIPAddress,FString UDPPort);
	TObjectPtr<AActor> BP_nConfigActor;
	TArray<TObjectPtr<ULiveLinkSourceFactory>> Factories;

	TArray<FString> GetAllLiveLinkSourceFactoriesNames();
	
	UFUNCTION(BlueprintCallable,Category="ConfigModule")
	FLiveLinkDataToDisplay PrintLiveLinkSourceInfo();

	UFUNCTION(BlueprintCallable, Category = "ConfigModule")
	void BindConnectionListeners();

	UFUNCTION(BlueprintCallable, Category = "ConfigModule")
	void HandleSourcesChanged(FGuid InGuid);

	UFUNCTION(BlueprintCallable, Category = "ConfigModule")
	void HandleSubjectsAdded(FLiveLinkSubjectKey InLiveLinkSubjectKey);

	UFUNCTION(BlueprintCallable, Category = "ConfigModule")
	void HandleSubjectsRemoved(FLiveLinkSubjectKey InLiveLinkSubjectKey);

	//void OnStaticDataAdded(FLiveLinkSubjectKey SubjectKey, TSubclassOf<ULiveLinkRole> Role, const FLiveLinkStaticDataStruct& StaticData);
	//void OnFrameDataAdded(FLiveLinkSubjectKey SubjectKey, TSubclassOf<ULiveLinkRole> Role, const FLiveLinkFrameDataStruct& FrameData);
	//void UpdateStatusDisplay(FColor Color);


	/*FDelegateHandle OnLiveLinkSourcesChangedHandle;
	FDelegateHandle OnLiveLinkSubjectsChangedHandle;

	FOnLiveLinkSourceChangedDelegate OnLiveLinkSourceChangedDelegate;
	FOnLiveLinkSubjectChangedDelegate OnLiveLinkSubjectChangedDelegate;*/


	FLiveLinkDataToDisplay LiveLinkDataToDisplay;

	ILiveLinkClient* InLiveLinkClientPtr{ nullptr };



	// The multicast delegate instance
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnConnectionUpdated OnConnectionUpdated;


};

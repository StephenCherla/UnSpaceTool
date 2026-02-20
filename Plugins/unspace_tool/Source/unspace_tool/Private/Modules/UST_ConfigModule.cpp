// Fill out your copyright notice in the Description page of Project Settings.


#include "Modules/UST_ConfigModule.h"
#include "utils/UST_EditorUtils.h"
#include "UObject/Object.h"
#include "Utils/UST_nDisplayConfigUtils.h"
#include "CineCameraActor.h"
#include "LiveLinkBlueprintLibrary.h"
#include "LiveLinkClient.h"
#include "LiveLinkComponentController.h"
#include "LiveLinkFreeD.h"
#include "LiveLinkClient.h"
#include "LiveLinkFreeDSource.h"
#include "LiveLinkFreeDSourceFactory.h"
#include "LiveLinkNatNetSourceFactory.h"
#include "LiveLinkPresetTypes.h"
#include "OptitrackNatnet.h"
#include "Utils/UST_StringUtils.h"

UUST_ConfigModule::UUST_ConfigModule()
{
	BindConnectionListeners();
}

void UUST_ConfigModule::CreateAndSave_nDisplayRootActorAsset(FnConfigAssetSettings AssetSettings)
{
	UUST_StringUtils::GetPluginVersionName();
	DisplayClustorBlueprintActor = UUST_nDisplayConfigUtils::CreateDisplayClusterBlueprintActor(AssetSettings);


	//PlaceConfigInEditorWorld(SaveFolderPath + "/" + Name);
}

void UUST_ConfigModule::PlaceConfigInEditorWorld(FString ConfigPath,int32 TrackingActorCount,FnConfigCineCameraSettings CineCameraSettings, FnConfigFrustumSettings FrustumSettings)
{
	UUST_StringUtils::GetPluginVersionName();
	FTransform WorldTransform = FTransform::Identity;
	UObject* nConfigObject = StaticLoadObject(UObject::StaticClass(), nullptr, *ConfigPath);
	UBlueprint* nConfigBP = Cast<UBlueprint>(nConfigObject);
	TSubclassOf<class UObject> nConfigBPClass = (UClass*)nConfigBP->GeneratedClass;
	TArray<AActor*> TrackingActors;
	for(int i=0;i<TrackingActorCount;i++)
	{
		FString TrackingActorName = FString::Printf(TEXT("TrackingActor_%d"),i);
		TrackingActors.Add(UUST_EditorUtils::SpawnEmptyActor(WorldTransform,TrackingActorName,AActor::StaticClass()));
	}
	AActor* LocationActor = UUST_EditorUtils::SpawnEmptyActor(WorldTransform,TEXT("LocationActor"),AActor::StaticClass());
	BP_nConfigActor =  UUST_EditorUtils::SpawnActorFromClass(WorldTransform,TEXT("uDisplay_Config"),nConfigBPClass);
	
	AActor* CineCameraActor = UUST_EditorUtils::SpawnActorFromClass(FTransform(FrustumSettings.CineCameraPosition),TEXT("CineCameraActor"),ACineCameraActor::StaticClass());
	UUST_EditorUtils::AttachActorToParentActor(BP_nConfigActor,LocationActor);
	for(int i=0;i<TrackingActors.Num();i++)
	{
		UUST_EditorUtils::AttachActorToParentActor(TrackingActors[i],BP_nConfigActor);
		if(i==0)
		{
			UUST_EditorUtils::AttachActorToParentActor(CineCameraActor,TrackingActors[i]);
			if(IsValid(TrackingActors[i]))
			{
				auto LiveLinkComponent = UUST_EditorUtils::AddComponentToActor(TrackingActors[i],ULiveLinkComponentController::StaticClass(),TEXT("LiveLinkComponent"));
			}
		}
	}
	UDisplayClusterICVFXCameraComponent* ICVFXComp = Cast<UDisplayClusterICVFXCameraComponent>(BP_nConfigActor->FindComponentByClass(UDisplayClusterICVFXCameraComponent::StaticClass()));
	if(IsValid(ICVFXComp) && IsValid(CineCameraActor))
	{
		ICVFXComp->CameraSettings.ExternalCameraActor = Cast<ACineCameraActor>(CineCameraActor);
	}
	
	UpdateCineCameraSettings(Cast<ACineCameraActor>(CineCameraActor),CineCameraSettings);
	UpdateFrustumValues(BP_nConfigActor, FrustumSettings);
	UUST_EditorUtils::ShowEditorNotification(TEXT("uDisplay_Config asset added to level"));
}

void UUST_ConfigModule::UpdateCineCameraSettings(ACineCameraActor* CineCameraActor, FnConfigCineCameraSettings CineCameraSettings)
{
	if(IsValid(CineCameraActor))
	{
		UCineCameraComponent* CineCameraComp = CineCameraActor->GetCineCameraComponent();
		CineCameraComp->Filmback.SensorWidth = CineCameraSettings.SensorWidth;
		CineCameraComp->Filmback.SensorHeight = CineCameraSettings.SensorHeight;

		CineCameraComp->LensSettings.MinFocalLength = CineCameraSettings.MinFocalLength;
		CineCameraComp->LensSettings.MaxFocalLength = CineCameraSettings.MaxFocalLength;

		CineCameraComp->LensSettings.MinFStop = CineCameraSettings.MinFStop;
		CineCameraComp->LensSettings.MaxFStop = CineCameraSettings.MaxFStop;

		CineCameraComp->FocusSettings.FocusMethod = CineCameraSettings.CameraTrackingMethod;
		CineCameraComp->FocusSettings.ManualFocusDistance = CineCameraSettings.ManualFocusDistance;

		CineCameraComp->CurrentAperture = CineCameraSettings.CurrentAperture;
		CineCameraComp->CurrentFocalLength = CineCameraSettings.CurrentFocalLength;
	}
}

void UUST_ConfigModule::UpdateFrustumValues(AActor* nConfigActor, FnConfigFrustumSettings FrustumSettings)
{
	if(IsValid(nConfigActor))
	{
		ADisplayClusterRootActor* nConfigActor_ = Cast<ADisplayClusterRootActor>(nConfigActor);
		if(IsValid(nConfigActor_))
		{
			//Viewport Screen Percentage Multiplier || Outer frustum percentage
			nConfigActor_->GetConfigData()->RenderFrameSettings.ClusterICVFXOuterViewportBufferRatioMult = FrustumSettings.ClusterICVFXOuterViewportBufferRatioMult;
			
			//Freeze Outer frustum
			nConfigActor_->GetConfigData()->StageSettings.bFreezeRenderOuterViewports = FrustumSettings.bFreezeRenderOuterViewports;

			//Enable OCIO Viewport
			nConfigActor_->GetConfigData()->StageSettings.ViewportOCIO.AllViewportsOCIOConfiguration.bIsEnabled = true;
			
			//Enable editor preview
			nConfigActor_->bPreviewEnable = FrustumSettings.bPreviewEnable;
			
			//Enable Inner frustum
			nConfigActor_->GetConfigData()->StageSettings.bEnableInnerFrustums = FrustumSettings.bEnableInnerFrustums;
			
			UDisplayClusterICVFXCameraComponent* ICVFXComp = Cast<UDisplayClusterICVFXCameraComponent>(BP_nConfigActor->FindComponentByClass(UDisplayClusterICVFXCameraComponent::StaticClass()));
			if(IsValid(ICVFXComp))
			{
				//Inner Frustum Screen Percentage
				ICVFXComp->CameraSettings.BufferRatio = FrustumSettings.BufferRatio;
				ICVFXComp->CameraSettings.CameraOCIO.AllNodesOCIOConfiguration.bIsEnabled = true;
			}
			//OCIO Configurations
			FString OCIOAssetPath = FrustumSettings.OCIOAssetPath;
			UOpenColorIOConfiguration* OpenColorIOConfig = Cast<UOpenColorIOConfiguration>(StaticLoadObject(UOpenColorIOConfiguration::StaticClass(), nullptr, *OCIOAssetPath));
			if(!IsValid(OpenColorIOConfig)) return;
			nConfigActor_->GetConfigData()->StageSettings.ViewportOCIO.AllViewportsOCIOConfiguration.ColorConfiguration.ConfigurationSource = OpenColorIOConfig;
			if(OpenColorIOConfig->DesiredColorSpaces.Num()>0)
				nConfigActor_->GetConfigData()->StageSettings.ViewportOCIO.AllViewportsOCIOConfiguration.ColorConfiguration.SourceColorSpace = OpenColorIOConfig->DesiredColorSpaces[0];
			if(OpenColorIOConfig->DesiredDisplayViews.Num()>0)
				nConfigActor_->GetConfigData()->StageSettings.ViewportOCIO.AllViewportsOCIOConfiguration.ColorConfiguration.DestinationDisplayView = OpenColorIOConfig->DesiredDisplayViews[0];
			
			if(IsValid(ICVFXComp))
			{
				ICVFXComp->CameraSettings.CameraOCIO.AllNodesOCIOConfiguration.ColorConfiguration.ConfigurationSource =  OpenColorIOConfig;
				if(OpenColorIOConfig->DesiredColorSpaces.Num()>0)
					ICVFXComp->CameraSettings.CameraOCIO.AllNodesOCIOConfiguration.ColorConfiguration.SourceColorSpace = OpenColorIOConfig->DesiredColorSpaces[0];
				if(OpenColorIOConfig->DesiredDisplayViews.Num()>0)
					ICVFXComp->CameraSettings.CameraOCIO.AllNodesOCIOConfiguration.ColorConfiguration.DestinationDisplayView = OpenColorIOConfig->DesiredDisplayViews[0];
			}
		}
	}
}

void UUST_ConfigModule::CreateOptiTrackSourceInLiveLinkPanel(FString InServerIPAddress,FString InClientIPAddress)
{
	FOptitrackLiveLinkSettings InSettings;
	InSettings.ConnectAutomatically = false;
	InSettings.ServerIpAddress = InServerIPAddress;
	InSettings.ClientIpAddress = InClientIPAddress;
	InSettings.ConnectionType = EOptitrackLiveLinkConnectionType::Unicast;
	IModularFeatures& ModularFeatures = IModularFeatures::Get();
	if (ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		FLiveLinkClient* LiveLinkClient = &ModularFeatures.GetModularFeature<FLiveLinkClient>(ILiveLinkClient::ModularFeatureName);
		if(LiveLinkClient)
		{
			TSharedPtr<ILiveLinkSource> NewSource = MakeShared<FLiveLinkNatNetSource>( InSettings );
			const FString ConnectionString = InSettings.ToString();
			if (NewSource.IsValid())
			{
				FGuid NewSourceGuid = LiveLinkClient->AddSource(NewSource);
				if (NewSourceGuid.IsValid())
				{
					if (ULiveLinkSourceSettings* Settings = LiveLinkClient->GetSourceSettings(NewSourceGuid))
					{
						Settings->ConnectionString = ConnectionString;
						Settings->Factory = ULiveLinkNatNetSourceFactory::StaticClass();
					}
				}
			}
		}
	}
}

void UUST_ConfigModule::CreateFreeDTrackSourceInLiveLinkPanel(FString InIPAddress,FString UDPPort)
{
	FLiveLinkFreeDConnectionSettings InSettings;
	InSettings.IPAddress = InIPAddress;
	InSettings.UDPPortNumber = FCString::Atoi(*UDPPort);
	IModularFeatures& ModularFeatures = IModularFeatures::Get();
	if (ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		FLiveLinkClient* LiveLinkClient = &ModularFeatures.GetModularFeature<FLiveLinkClient>(ILiveLinkClient::ModularFeatureName);
		if(LiveLinkClient)
		{
			TSharedPtr<ILiveLinkSource> NewSource = MakeShared<FLiveLinkFreeDSource>( InSettings );
			FString ConnectionString;
			FLiveLinkFreeDConnectionSettings::StaticStruct()->ExportText(ConnectionString, &InSettings, nullptr, nullptr, PPF_None, nullptr);
			if (NewSource.IsValid())
			{
				FGuid NewSourceGuid = LiveLinkClient->AddSource(NewSource);
				if (NewSourceGuid.IsValid())
				{
					if (ULiveLinkSourceSettings* Settings = LiveLinkClient->GetSourceSettings(NewSourceGuid))
					{
						Settings->ConnectionString = ConnectionString;
						Settings->Factory = ULiveLinkFreeDSourceFactory::StaticClass();
					}
				}
			}
		}
	}
}

TArray<FString> UUST_ConfigModule::GetAllLiveLinkSourceFactoriesNames()
{
	TArray<FString> DisplayNames;
	TArray<UClass*> Results;
	GetDerivedClasses(ULiveLinkSourceFactory::StaticClass(), Results, true);
	for (UClass* SourceFactory : Results)
	{
		DisplayNames.Add(SourceFactory->GetDisplayNameText().ToString());
	}
	return DisplayNames;
}

void UUST_ConfigModule::BindConnectionListeners() {
	if (!InLiveLinkClientPtr) {
		InLiveLinkClientPtr = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
		/*OnLiveLinkSourcesChangedHandle = InLiveLinkClientPtr->OnLiveLinkSubjectsChanged().AddUObject(this, &UUST_ConfigModule::HandleSourcesChanged);
		OnLiveLinkSubjectsChangedHandle = InLiveLinkClientPtr->OnLiveLinkSubjectsChanged().AddUObject(this, &UUST_ConfigModule::HandleSubjectsChanged);*/

		InLiveLinkClientPtr->OnLiveLinkSourceAdded().AddUObject(this, &UUST_ConfigModule::HandleSourcesChanged);
		InLiveLinkClientPtr->OnLiveLinkSourceRemoved().AddUObject(this, &UUST_ConfigModule::HandleSourcesChanged);


		InLiveLinkClientPtr->OnLiveLinkSubjectAdded().AddUObject(this, &UUST_ConfigModule::HandleSubjectsAdded);
		InLiveLinkClientPtr->OnLiveLinkSubjectRemoved().AddUObject(this, &UUST_ConfigModule::HandleSubjectsRemoved);
	}
}

FLiveLinkDataToDisplay UUST_ConfigModule::PrintLiveLinkSourceInfo()
{
	FLiveLinkClient& InLiveLinkClient = IModularFeatures::Get().GetModularFeature<FLiveLinkClient>(ILiveLinkClient::ModularFeatureName);
	
	TArray<FGuid> SourceIDs = InLiveLinkClient.GetSources(true);
	LiveLinkDataToDisplay.UConnectionsData.Empty();
	for(auto SourceID:SourceIDs)
	{
		FLiveLinkConnectionsDisplay LiveLinkConnectionsDisplay;
		LiveLinkConnectionsDisplay.SourceStatus = InLiveLinkClient.GetSourceStatus(SourceID).ToString();
		LiveLinkConnectionsDisplay.SourceType = InLiveLinkClient.GetSourceType(SourceID).ToString();
		LiveLinkConnectionsDisplay.SourceMachineName = InLiveLinkClient.GetSourceMachineName(SourceID).ToString();
		LiveLinkDataToDisplay.UConnectionsData.Add(LiveLinkConnectionsDisplay);
		InLiveLinkClient.GetSubjects(true,true);
			
	}
	
	TArray<FLiveLinkSubjectKey> LiveLinkSubjects = InLiveLinkClient.GetSubjects(true,true);
	LiveLinkDataToDisplay.UDevicesData.Empty();
	for(auto LiveLinkSubject:LiveLinkSubjects)
	{
		FLiveLinkDevicesDisplay LiveLinkDevicesDisplay;
		LiveLinkDevicesDisplay.SubjectName = LiveLinkSubject.SubjectName.Name.ToString();
		//Replace this with ULiveLinkBlueprintLibrary::EvaluateLiveLinkFrame later
		LiveLinkDevicesDisplay.Status = ULiveLinkBlueprintLibrary::IsLiveLinkSubjectEnabled(LiveLinkSubject.SubjectName);
		LiveLinkDevicesDisplay.Role = InLiveLinkClient.GetSubjectRole(LiveLinkSubject)->GetName();
		LiveLinkDataToDisplay.UDevicesData.Add(LiveLinkDevicesDisplay);
	}
	return LiveLinkDataToDisplay;
}

void UUST_ConfigModule::HandleSourcesChanged(FGuid InGuid)
{
	
	// Handle the event when LiveLink sources change
	UE_LOG(LogTemp, Log, TEXT("LiveLink sources have changed."));
	LiveLinkDataToDisplay = PrintLiveLinkSourceInfo();
	OnConnectionUpdated.Broadcast(LiveLinkDataToDisplay);
}

void UUST_ConfigModule::HandleSubjectsAdded(FLiveLinkSubjectKey InLiveLinkSubjectKey)
{

	// Handle the event when LiveLink subjects change
	UE_LOG(LogTemp, Log, TEXT("LiveLink subjects have changed."));

	if (InLiveLinkClientPtr) {
		/*FDelegateHandle StaticDataAddedHandle;
		FDelegateHandle FrameDataAddedHandle;
		TSubclassOf<ULiveLinkRole> SubjectRole;


		bool bSuccess = InLiveLinkClientPtr->RegisterForSubjectFrames(
			FLiveLinkSubjectName(InLiveLinkSubjectKey.SubjectName),
			FOnLiveLinkSubjectStaticDataAdded::FDelegate::CreateRaw(this, &UUST_ConfigModule::OnStaticDataAdded),
			FOnLiveLinkSubjectFrameDataAdded::FDelegate::CreateRaw(this, &UUST_ConfigModule::OnFrameDataAdded),
			StaticDataAddedHandle,
			FrameDataAddedHandle,
			SubjectRole
		);*/
	}


	LiveLinkDataToDisplay = PrintLiveLinkSourceInfo();
	OnConnectionUpdated.Broadcast(LiveLinkDataToDisplay);
}

void UUST_ConfigModule::HandleSubjectsRemoved(FLiveLinkSubjectKey InLiveLinkSubjectKey)
{
	if (InLiveLinkClientPtr) {
		//InLiveLinkClientPtr->UnregisterSubjectFramesHandle(InLiveLinkSubjectKey.SubjectName);
	}

	// Handle the event when LiveLink subjects change
	UE_LOG(LogTemp, Log, TEXT("LiveLink subjects have changed."));
	LiveLinkDataToDisplay = PrintLiveLinkSourceInfo();
	OnConnectionUpdated.Broadcast(LiveLinkDataToDisplay);
}

//void UUST_ConfigModule::OnStaticDataAdded(FLiveLinkSubjectKey SubjectKey, TSubclassOf<ULiveLinkRole> Role, const FLiveLinkStaticDataStruct& StaticData)
//{
//
//	UE_LOG(LogTemp, Log, TEXT("Received static data for subject: %s"), *SubjectKey.SubjectName.ToString());
//	
//}
//
//void UUST_ConfigModule::OnFrameDataAdded(FLiveLinkSubjectKey SubjectKey, TSubclassOf<ULiveLinkRole> Role, const FLiveLinkFrameDataStruct& FrameData)
//{
//	
//	UE_LOG(LogTemp, Log, TEXT("Received frame data for subject: %s"), *SubjectKey.SubjectName.ToString());
//
//	
//	if (const FLiveLinkBaseFrameData* BaseFrameData = FrameData.Cast<FLiveLinkBaseFrameData>())
//	{
//		
//		
//	}
//}
//
//void UUST_ConfigModule::UpdateStatusDisplay(FColor Color)
//{
//
//}

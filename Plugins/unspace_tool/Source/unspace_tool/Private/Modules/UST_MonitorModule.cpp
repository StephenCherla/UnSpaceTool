// Fill out your copyright notice in the Description page of Project Settings.


#include "Modules/UST_MonitorModule.h"
#include "CineCameraComponent.h"
#include "Components/DisplayClusterICVFXCameraComponent.h"


void UUST_MonitorModule::UpdateCineCameraSettings(ACineCameraActor* CineCameraActor,
                                                  FnConfigCineCameraSettings CineCameraSettings)
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

void UUST_MonitorModule::UpdateUConfigSettings(ADisplayClusterRootActor* nConfigActor,
	FnConfigMonitorSettings nConfigMonitorSettings)
{
	if(IsValid(nConfigActor))
	{
		ADisplayClusterRootActor* nConfigActor_ = Cast<ADisplayClusterRootActor>(nConfigActor);
		if(IsValid(nConfigActor_))
		{
			//Viewport Screen Percentage Multiplier || Outer frustum percentage
			nConfigActor_->GetConfigData()->RenderFrameSettings.ClusterICVFXOuterViewportBufferRatioMult = nConfigMonitorSettings.OuterFrustumScreenPercentage;

			//Viewport Screen Percentage Multiplier || Inner frustum percentage
			nConfigActor_->GetConfigData()->RenderFrameSettings.ClusterICVFXInnerFrustumBufferRatioMult = nConfigMonitorSettings.InnerFrustumScreenPercentage;
			
			//Freeze Outer frustum
			nConfigActor_->GetConfigData()->StageSettings.bFreezeRenderOuterViewports = nConfigMonitorSettings.bFreezeOuterFrustrum;
			
			//Enable Inner frustum
			nConfigActor_->GetConfigData()->StageSettings.bEnableInnerFrustums = nConfigMonitorSettings.bEnableInnerFrustum;

			UDisplayClusterICVFXCameraComponent* ICVFXComp = Cast<UDisplayClusterICVFXCameraComponent>(nConfigActor_->FindComponentByClass(UDisplayClusterICVFXCameraComponent::StaticClass()));
			if(IsValid(ICVFXComp))
			{
				//SoftEdge Top/Bottom, Sides
				ICVFXComp->CameraSettings.SoftEdge.Vertical = nConfigMonitorSettings.IF_SoftEdge_TOP_BOTTOM;
				ICVFXComp->CameraSettings.SoftEdge.Horizontal = nConfigMonitorSettings.IF_SoftEdge_SIDES;

				//Overscan values
				ICVFXComp->CameraSettings.CustomFrustum.bEnable = nConfigMonitorSettings.bEnableOverScan;
				ICVFXComp->CameraSettings.CustomFrustum.Left = nConfigMonitorSettings.IF_OVERSCAN_LEFT;
				ICVFXComp->CameraSettings.CustomFrustum.Right = nConfigMonitorSettings.IF_OVERSCAN_RIGHT;
				ICVFXComp->CameraSettings.CustomFrustum.Top = nConfigMonitorSettings.IF_OVERSCAN_TOP;
				ICVFXComp->CameraSettings.CustomFrustum.Bottom = nConfigMonitorSettings.IF_OVERSCAN_BOTTOM;
			}
		}
	}
}

FnConfigCineCameraSettings UUST_MonitorModule::FetchCineCameraSettings(ACineCameraActor* CineCameraActor)
{
	FnConfigCineCameraSettings CineCameraSettings;
	if(IsValid(CineCameraActor))
	{
		UCineCameraComponent* CineCameraComp = CineCameraActor->GetCineCameraComponent();
		CineCameraSettings.SensorWidth = CineCameraComp->Filmback.SensorWidth;
		CineCameraSettings.SensorHeight = CineCameraComp->Filmback.SensorHeight ;

		CineCameraSettings.MinFocalLength = CineCameraComp->LensSettings.MinFocalLength ;
		CineCameraSettings.MaxFocalLength= CineCameraComp->LensSettings.MaxFocalLength ;

		CineCameraSettings.MinFStop = CineCameraComp->LensSettings.MinFStop ;
		CineCameraSettings.MaxFStop = CineCameraComp->LensSettings.MaxFStop ;

		CineCameraSettings.CameraTrackingMethod = CineCameraComp->FocusSettings.FocusMethod ;
		CineCameraSettings.ManualFocusDistance = CineCameraComp->FocusSettings.ManualFocusDistance;

		CineCameraSettings.CurrentAperture = CineCameraComp->CurrentAperture ;
		CineCameraSettings.CurrentFocalLength = CineCameraComp->CurrentFocalLength;
	}
	return CineCameraSettings;
}

FnConfigMonitorSettings UUST_MonitorModule::FetchConfigMonitorSettings(ADisplayClusterRootActor* nConfigActor)
{
	FnConfigMonitorSettings nConfigMonitorSettings;
	if(IsValid(nConfigActor))
	{
		ADisplayClusterRootActor* nConfigActor_ = Cast<ADisplayClusterRootActor>(nConfigActor);
		if(IsValid(nConfigActor_))
		{
			//Viewport Screen Percentage Multiplier || Outer frustum percentage
			nConfigMonitorSettings.OuterFrustumScreenPercentage = nConfigActor_->GetConfigData()->RenderFrameSettings.ClusterICVFXOuterViewportBufferRatioMult ;

			//Viewport Screen Percentage Multiplier || Inner frustum percentage
			nConfigMonitorSettings.InnerFrustumScreenPercentage = nConfigActor_->GetConfigData()->RenderFrameSettings.ClusterICVFXInnerFrustumBufferRatioMult;
			
			//Freeze Outer frustum
			nConfigMonitorSettings.bFreezeOuterFrustrum = nConfigActor_->GetConfigData()->StageSettings.bFreezeRenderOuterViewports;
			
			//Enable Inner frustum
			nConfigMonitorSettings.bEnableInnerFrustum = nConfigActor_->GetConfigData()->StageSettings.bEnableInnerFrustums;

			UDisplayClusterICVFXCameraComponent* ICVFXComp = Cast<UDisplayClusterICVFXCameraComponent>(nConfigActor_->FindComponentByClass(UDisplayClusterICVFXCameraComponent::StaticClass()));
			if(IsValid(ICVFXComp))
			{
				//SoftEdge Top/Bottom, Sides
				nConfigMonitorSettings.IF_SoftEdge_TOP_BOTTOM = ICVFXComp->CameraSettings.SoftEdge.Vertical;
				nConfigMonitorSettings.IF_SoftEdge_SIDES = ICVFXComp->CameraSettings.SoftEdge.Horizontal;

				//Overscan values
				nConfigMonitorSettings.bEnableOverScan = ICVFXComp->CameraSettings.CustomFrustum.bEnable;
				nConfigMonitorSettings.IF_OVERSCAN_LEFT = ICVFXComp->CameraSettings.CustomFrustum.Left;
				nConfigMonitorSettings.IF_OVERSCAN_RIGHT = ICVFXComp->CameraSettings.CustomFrustum.Right;
				nConfigMonitorSettings.IF_OVERSCAN_TOP = ICVFXComp->CameraSettings.CustomFrustum.Top;
				nConfigMonitorSettings.IF_OVERSCAN_BOTTOM = ICVFXComp->CameraSettings.CustomFrustum.Bottom;
			}
		}
	}
	return nConfigMonitorSettings;
}

FString UUST_MonitorModule::GetSwitchboardPath()
{
	using namespace UE::UST_Monitor::Private;
	const FString SwitchboardPath = ConcatPaths(FPaths::EngineDir(),
		"Binaries", "Win64");
	
	FString ExePath;
	#if PLATFORM_WINDOWS
		ExePath = SwitchboardPath / TEXT("SwitchboardListener.exe");
	#endif
	return ExePath;
}

FString UUST_MonitorModule::GetStageMonitorPath()
{
	using namespace UE::UST_Monitor::Private;
	FString SwitchboardPath = ConcatPaths(FPaths::EnginePluginsDir(),
		"VirtualProduction", "Switchboard", "Source", "Switchboard");
	
	FString ExePath;
#if PLATFORM_WINDOWS
	ExePath = SwitchboardPath / TEXT("switchboard.bat");
#elif PLATFORM_LINUX
	ExePath = SwitchboardPath / TEXT("switchboard.sh");
#endif

	return ExePath;
}

FString UUST_MonitorModule::GetPythonExePath()
{
	using namespace UE::UST_Monitor::Private;

	FString PythonExe = ConcatPaths(
		FPaths::EngineDir(), "Binaries", "ThirdParty", "Python3",
		FPlatformProcess::GetBinariesSubdirectory(),
		#if PLATFORM_WINDOWS
				"pythonw.exe"
		#else
				"bin", "python3"
		#endif
	);

	return PythonExe;
}

bool UUST_MonitorModule::LaunchProcess(const FString& ProcessPath, const FString& Args)
{
	if (ProcHandle.IsValid())
	{
		// Not currently designed to Run() twice.
		return ensure(false);
	}

	if (!FPaths::FileExists(ProcessPath))
	{
		return false;
	}

	const bool bLaunchDetached = false;
	const bool bLaunchHidden = true;
	const bool bLaunchReallyHidden = true;
	uint32 OutProcessId = 0;
	const int32 PriorityModifier = 0;
	const TCHAR* WorkingDirectory = nullptr;
	FPlatformProcess::CreatePipe(StdoutParentReadPipe, StdoutChildWritePipe);

	ProcHandle = FPlatformProcess::CreateProc(*ProcessPath, *Args, bLaunchDetached, bLaunchHidden, bLaunchReallyHidden, &OutProcessId, PriorityModifier, WorkingDirectory, StdoutChildWritePipe);
	return ProcHandle.IsValid();
}

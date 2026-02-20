// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "DisplayClusterRootActor.h"
#include "DataClasses/nConfigSpawnData.h"
#include "UObject/Object.h"
#include "UST_MonitorModule.generated.h"

namespace UE::UST_Monitor::Private
{
	template <typename... TPaths>
	FString ConcatPaths(FString BaseDir, TPaths... InPaths)
	{
		return (FPaths::ConvertRelativePathToFull(BaseDir) / ... / InPaths);
	}
}

/**
 * 
 */
UCLASS()
class UNSPACE_TOOL_API UUST_MonitorModule : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category= "UnSpaceTool | Monitor")
	void UpdateCineCameraSettings(ACineCameraActor* CineCameraActor,FnConfigCineCameraSettings CineCameraSettings);
	UFUNCTION(BlueprintCallable, Category= "UnSpaceTool | Monitor")
	void UpdateUConfigSettings(ADisplayClusterRootActor* nConfigActor,FnConfigMonitorSettings nConfigMonitorSettings);

	UFUNCTION(BlueprintCallable, Category= "UnSpaceTool | Monitor")
	FnConfigCineCameraSettings FetchCineCameraSettings(ACineCameraActor* CineCameraActor);
	UFUNCTION(BlueprintCallable, Category= "UnSpaceTool | Monitor")
	FnConfigMonitorSettings FetchConfigMonitorSettings(ADisplayClusterRootActor* nConfigActor);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Monitor")
	FString GetSwitchboardPath();

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Monitor")
	FString GetStageMonitorPath();
	
	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Monitor")
	FString GetPythonExePath();

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool | Monitor")
	bool LaunchProcess(const FString& ProcessPath, const FString& Args);

	FProcHandle ProcHandle;

	void* StdoutChildWritePipe = nullptr;
	void* StdoutParentReadPipe = nullptr;
	TArray<uint8> StdoutBuf;
	TOptional<int32> ReturnCode;
};

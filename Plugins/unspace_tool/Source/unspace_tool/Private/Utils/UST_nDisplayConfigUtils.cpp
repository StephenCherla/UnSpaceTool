// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/UST_nDisplayConfigUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "DisplayClusterConfiguratorFactory.h"
#include "KismetCompilerModule.h"
#include <SSCSEditor.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <Kismet2/KismetEditorUtilities.h>
#include "Components/DisplayClusterCameraComponent.h"
#include "Components/DisplayClusterScreenComponent.h"
#include "ClusterConfiguration/DisplayClusterConfiguratorClusterUtils.h"
#include "DisplayClusterConfigurationTypes_Base.h"
#include "CoreFwd.h"
#include "DisplayClusterProjectionStrings.h"
#include "DisplayClusterRootActor.h"
#include "EditorAssetLibrary.h"
#include "Logging/StructuredLog.h"
#include "Utils/UST_EditorUtils.h"
#include "Utils/UST_MeshUtils.h"
#include "Utils/UST_StringUtils.h"

UDisplayClusterBlueprint* UUST_nDisplayConfigUtils::CreateDisplayClusterBlueprintActor(FnConfigAssetSettings AssetSettings)
{
	UObject* Asset = nullptr;
	UPackage* Package = nullptr;

	FString PackageName = AssetSettings.SaveFolderPath + "/" + AssetSettings.AssetName;

	UDisplayClusterBlueprint* DisplayClusterBlueprint = nullptr;

	if (UDisplayClusterConfiguratorFactory* Factory = NewObject<UDisplayClusterConfiguratorFactory>())
	{
		Package = CreatePackage(*PackageName);
		if (Package)
		{
			constexpr EObjectFlags ObjectFlags = RF_Transient | RF_Public;
			Asset = Factory->FactoryCreateNew(UDisplayClusterBlueprint::StaticClass(), Package, *FPaths::GetBaseFilename(PackageName), ObjectFlags, nullptr, GWarn);

			if (Asset)
			{
				DisplayClusterBlueprint = Cast<UDisplayClusterBlueprint>(Asset);
				USCS_Node* IcvfxNode;
				USCS_Node* uTransformNode;
				AddComponentToBlueprint(Cast<UDisplayClusterBlueprint>(Asset), UDisplayClusterICVFXCameraComponent::StaticClass(), FName("ICVFXCamera"),IcvfxNode);
				AddComponentToBlueprint(Cast<UDisplayClusterBlueprint>(Asset), UDisplayClusterXformComponent::StaticClass(), FName("uDisplayTransform"),uTransformNode);
				Remove_nDisplayScreenComponent(Cast<UDisplayClusterBlueprint>(Asset));
				set_nDisplayDefaultViewpointLocation(Cast<UDisplayClusterBlueprint>(Asset),AssetSettings.DefaultViewPointLocation);
				TArray<UStaticMesh*> StaticMeshesToAdd =  UUST_MeshUtils::GetStaticMeshesInFolder(AssetSettings.MeshFolderPath);
				UDisplayClusterConfigurationData* AssetClusterConfigData = UUST_nDisplayConfigUtils::GetConfigData(DisplayClusterBlueprint);

				UDisplayClusterConfigurationCluster* AssetRootCluster = UUST_nDisplayConfigUtils::GetCluster(AssetClusterConfigData);
				UUST_nDisplayConfigUtils::SelectSyncType(DisplayClusterBlueprint,AssetRootCluster,AssetSettings.SyncType);
				//UDisplayClusterConfigurationHostDisplayData* AssetHostDisplayData = UUST_nDisplayConfigUtils::AddHostDisplayDataToCluster(DisplayClustorBlueprintActor, AssetRootCluster, TestHostIP, false);
				FVector2D WindowsResolution = AssetSettings.WindowsResolution;
				FVector2D ViewportResolution = AssetSettings.ViewportResolution;
				int32 Counter = 0;
				// Get the proper values from IP address
				int32 IPAddressCounter = 1;
				FString IPAddressPrefix = UUST_StringUtils::RemoveLastDigitFromIP(AssetSettings.IPAddress,IPAddressCounter);
				TArray<FString> MeshNames;
				for(auto InStaticMesh:StaticMeshesToAdd)
				{
					UE_LOG(LogTemp,Warning,TEXT("InStaticMesh Name : %s"),*InStaticMesh->GetName());
					USCS_Node* staticMeshCompNode;
					UActorComponent* StaticMeshComp = UUST_nDisplayConfigUtils::AddComponentToBlueprint(DisplayClusterBlueprint,UStaticMeshComponent::StaticClass(),*InStaticMesh->GetName(),staticMeshCompNode);
					Cast<UStaticMeshComponent>(StaticMeshComp)->SetStaticMesh(InStaticMesh);
					uTransformNode->AddChildNode(staticMeshCompNode);
					FString NodeName = FString::Printf(TEXT("Node_%d"),Counter);
					FString IPAddress = FString::Printf(TEXT("%s%d"),*IPAddressPrefix,(Counter+IPAddressCounter));
					UDisplayClusterConfigurationClusterNode* AssetClusterNode = UUST_nDisplayConfigUtils::AddClusterNodeToCluster(DisplayClusterBlueprint, AssetRootCluster, NodeName, IPAddress, WindowsResolution,false);
					UDisplayClusterConfigurationViewport* AssetViewport = UUST_nDisplayConfigUtils::AddViewportToClusterNode(DisplayClusterBlueprint, AssetClusterNode, TEXT("Viewport"),ViewportResolution, false);
					TMap<FString,FString> ViewportParams;
					ViewportParams.Add("section_index","0.0");
					ViewportParams.Add("base_uv_index","-1.0");
					ViewportParams.Add("chromakey_uv_index","-1.0");
					ViewportParams.Add("mesh_component",*InStaticMesh->GetName());
					AssetViewport->ProjectionPolicy.Type = DisplayClusterProjectionStrings::projection::Mesh;
					AssetViewport->ProjectionPolicy.Parameters = ViewportParams;
					Counter++;
				}
				CompileBlueprint(DisplayClusterBlueprint);

				FAssetRegistryModule::AssetCreated(Asset);
				UEditorAssetLibrary::SaveLoadedAsset(Package);
				UE_LOG(LogTemp, Warning, TEXT("UDisplayClusterBlueprint asset created"));
				UUST_EditorUtils::ShowEditorNotification(TEXT("DisplayClusterBlueprint asset created"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create asset"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create package"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create factory"));
	}



	// Add Required Components Here

	// Prevent garbage collection of the asset and package
	Asset->AddToRoot();
	Package->AddToRoot();

	return DisplayClusterBlueprint;
}

UActorComponent* UUST_nDisplayConfigUtils::AddComponentToBlueprint(UDisplayClusterBlueprint* NewBlueprint, UClass* NewComponentClass, FName VariableName, USCS_Node* &OutNode)
{
	//UDisplayClusterICVFXCameraComponent::StaticClass()
	check(NewBlueprint != nullptr && NewBlueprint->SimpleConstructionScript != nullptr);
	NewBlueprint->Modify();

	const FName NewVariableName = (VariableName == "" ? NAME_None : VariableName);
	// Create a new SCS_Node for the component
	USCS_Node* NewNode = NewBlueprint->SimpleConstructionScript->CreateNode(NewComponentClass, NewVariableName);

	// Get the construction script
	UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(NewBlueprint->GeneratedClass);
	USimpleConstructionScript* ConstructionScript = GeneratedClass->SimpleConstructionScript;

	if (ConstructionScript)
	{
		// Add the new node to the construction script
		ConstructionScript->Modify();
		ConstructionScript->AddNode(NewNode);
	}
	OutNode = NewNode;
	return NewNode->ComponentTemplate;
}

UActorComponent* UUST_nDisplayConfigUtils::GetFirstComponentByClass(UDisplayClusterBlueprint* NewBlueprint,
	UClass* NewComponentClass)
{
	check(NewBlueprint != nullptr && NewBlueprint->SimpleConstructionScript != nullptr);
	// Get the construction script
	UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(NewBlueprint->GeneratedClass);
	USimpleConstructionScript* ConstructionScript = GeneratedClass->SimpleConstructionScript;
	USCS_Node* NodeToFetch = nullptr;
	if (ConstructionScript)
	{
		for (auto Node : ConstructionScript->GetAllNodes())
		{
			if(Node->ComponentClass == NewComponentClass)
			{
				NodeToFetch = Node;
				break;
			}
		}
	}
	UActorComponent* ActorComponentToSend = Cast<UActorComponent>(NodeToFetch->GetActualComponentTemplate(GeneratedClass));
	return ActorComponentToSend;
}

void UUST_nDisplayConfigUtils::Remove_nDisplayScreenComponent(UDisplayClusterBlueprint* NewBlueprint)
{
	check(NewBlueprint != nullptr && NewBlueprint->SimpleConstructionScript != nullptr);
	NewBlueprint->Modify();

	// Get the construction script
	UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(NewBlueprint->GeneratedClass);
	USimpleConstructionScript* ConstructionScript = GeneratedClass->SimpleConstructionScript;

	if (ConstructionScript)
	{
		// Remove the new node to the construction script
		ConstructionScript->Modify();
		USCS_Node* NodeToRemove = nullptr;
		for (auto Node : ConstructionScript->GetAllNodes())
		{
			if (UDisplayClusterScreenComponent* DC_SceneComponent = Cast<UDisplayClusterScreenComponent>(Node->GetActualComponentTemplate(GeneratedClass)))
			{
				NodeToRemove = Node;
				break;
			}
		}
		if (NodeToRemove!= nullptr) {
			ConstructionScript->RemoveNode(NodeToRemove);

			// Compile the construction script
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(NewBlueprint);
			FBlueprintEditorUtils::PostEditChangeBlueprintActors(NewBlueprint);

			FCompilerResultsLog LogResults;
			FKismetEditorUtilities::CompileBlueprint(NewBlueprint, EBlueprintCompileOptions::None, &LogResults);
			NewBlueprint->MarkPackageDirty();
		}
	}
}

void UUST_nDisplayConfigUtils::set_nDisplayDefaultViewpointLocation(UDisplayClusterBlueprint* NewBlueprint,
	FVector DefaultLocation)
{
	check(NewBlueprint != nullptr && NewBlueprint->SimpleConstructionScript != nullptr);
	NewBlueprint->Modify();
	UE_LOG(LogTemp,Warning,TEXT("set_nDisplayDefaultViewpointLocation Outer class %s"), *NewBlueprint->GetOuter()->GetName())
	UClass* Class = NewBlueprint->GeneratedClass;
	AActor* CDO = Class->GetDefaultObject<AActor>();
	ADisplayClusterRootActor* RootActor = Cast<ADisplayClusterRootActor>(CDO);
	if(IsValid(RootActor))
	{
		UE_LOG(LogTemp,Warning,TEXT("set_nDisplayDefaultViewpointLocation called"));
		RootActor->GetDefaultCamera()->SetRelativeLocation(DefaultLocation);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("set_nDisplayDefaultViewpointLocation error"));
	}
}

void UUST_nDisplayConfigUtils::set_uDisplayTransformRotation(UDisplayClusterBlueprint* NewBlueprint, FQuat& quaterion)
{
	check(NewBlueprint != nullptr && NewBlueprint->SimpleConstructionScript != nullptr);
	NewBlueprint->Modify();

	UClass* Class = NewBlueprint->GeneratedClass;
	AActor* CDO = Class->GetDefaultObject<AActor>();
	ADisplayClusterRootActor* RootActor = Cast<ADisplayClusterRootActor>(CDO);
	if (IsValid(RootActor))
	{
		for (auto comp : RootActor->GetComponents()) {
			UE_LOG(LogTemp, Warning, TEXT("Found  comp %s"), *comp->GetName());
			if (auto uDisplayTransformComp = Cast<UDisplayClusterXformComponent>(comp)) {
				UE_LOG(LogTemp, Warning, TEXT("Found  uDisplayTransformComp"));
				uDisplayTransformComp->SetRelativeRotation(quaterion);
				break;
			}
			
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("set_uDisplayTransformRotation error"));
	}
}

UDisplayClusterICVFXCameraComponent* UUST_nDisplayConfigUtils::CreateICVFXCameraComponent()
{
	UDisplayClusterICVFXCameraComponent* newICVFXCameraComponent = NewObject<UDisplayClusterICVFXCameraComponent>();
	if (IsValid(newICVFXCameraComponent))
	{
		newICVFXCameraComponent->RegisterComponent();
		return newICVFXCameraComponent;
	}
	return NULL;
}

UDisplayClusterConfigurationData* UUST_nDisplayConfigUtils::GetConfigData(UDisplayClusterBlueprint* ClusterAssetBlueprint)
{
	if (ClusterAssetBlueprint)
	{
		UDisplayClusterConfigurationData* AssetClusterConfig = ClusterAssetBlueprint->GetOrLoadConfig();
		if (AssetClusterConfig)
		{
			return AssetClusterConfig;
		}
	}
	return NULL;
}

UDisplayClusterConfigurationCluster* UUST_nDisplayConfigUtils::GetCluster(UDisplayClusterConfigurationData* ClusterConfigData)
{
	if (ClusterConfigData)
	{
		return ClusterConfigData->Cluster;	
	}
	return NULL;
}

UDisplayClusterConfigurationClusterNode* UUST_nDisplayConfigUtils::AddClusterNodeToCluster(UBlueprint* Blueprint, UDisplayClusterConfigurationCluster* RootCluster, FString Name, FString Host, FVector2D WindowsResolution, bool bCallPostEditChange)
{
	UDisplayClusterConfigurationClusterNode* NodeTemplate = NewObject<UDisplayClusterConfigurationClusterNode>(Blueprint);
	
	const FDisplayClusterConfigurationRectangle PresetRect(0,0,WindowsResolution.X,WindowsResolution.Y);
	NodeTemplate->WindowRect = FDisplayClusterConfigurationRectangle(PresetRect);
	NodeTemplate->Host = Host;

	const FVector2D DesiredPosition = FVector2D(NodeTemplate->WindowRect.X, NodeTemplate->WindowRect.Y);
	const FVector2D DesiredSize = FVector2D(NodeTemplate->WindowRect.W, NodeTemplate->WindowRect.H);
	/*FVector2D NewPosition = FDisplayClusterConfiguratorClusterUtils::FindNextAvailablePositionForClusterNode(RootCluster, NodeTemplate->Host, DesiredPosition, DesiredSize);

	NodeTemplate->WindowRect.X = NewPosition.X;
	NodeTemplate->WindowRect.Y = NewPosition.Y;*/

	// This must be set to avoid errors about illegal cross-package references
	NodeTemplate->SetFlags(RF_Transactional);

	UDisplayClusterConfigurationClusterNode* NewNode = FDisplayClusterConfiguratorClusterUtils::AddClusterNodeToCluster(NodeTemplate, RootCluster, Name);
	if(RootCluster->Nodes.Num()==1)
	{
		FDisplayClusterConfiguratorClusterUtils::SetClusterNodeAsPrimary(NewNode);
	}
	// Node template is no longer needed, leave it to be cleaned up
	NodeTemplate->MarkAsGarbage();

	if (bCallPostEditChange)
	{
		// Trigger Blueprint updates as if we were in an editor. This will re-run construction scripts.
		FBlueprintEditorUtils::PostEditChangeBlueprintActors(Blueprint);
	}
	return NewNode;
}

UDisplayClusterConfigurationViewport* UUST_nDisplayConfigUtils::AddViewportToClusterNode(UBlueprint* Blueprint, UDisplayClusterConfigurationClusterNode* Node, FString Name, FVector2D ViewportResolution, bool bCallPostEditChange)
{
	UDisplayClusterConfigurationViewport* ViewportTemplate = NewObject<UDisplayClusterConfigurationViewport>(Blueprint);
	const FDisplayClusterConfigurationRectangle PresetRect(0,0,ViewportResolution.X,ViewportResolution.Y);
	ViewportTemplate->Region = FDisplayClusterConfigurationRectangle(PresetRect);
	// This must be set to avoid errors about illegal cross-package references
	ViewportTemplate->SetFlags(RF_Transactional);

	UDisplayClusterConfigurationViewport* NewViewport = FDisplayClusterConfiguratorClusterUtils::AddViewportToClusterNode(ViewportTemplate, Node, Name);

	// Node template is no longer needed, leave it to be cleaned up
	ViewportTemplate->MarkAsGarbage();

	if (bCallPostEditChange)
	{
		// Trigger Blueprint updates as if we were in an editor. This will re-run construction scripts.
		FBlueprintEditorUtils::PostEditChangeBlueprintActors(Blueprint);
	}
	CompileBlueprint(Blueprint);
	return NewViewport;
}

void UUST_nDisplayConfigUtils::SelectSyncType(UBlueprint* Blueprint, UDisplayClusterConfigurationCluster* RootCluster,
	EClusterSyncType SyncType)
{
	if(RootCluster)
	{
		switch (SyncType)
		{
		case Nvidia:
			RootCluster->Sync.RenderSyncPolicy.Type = FString::Printf(TEXT("Nvidia"));
			break;
		case Ethernet:
			RootCluster->Sync.RenderSyncPolicy.Type = FString::Printf(TEXT("Ethernet"));
			break;
		case None:
			RootCluster->Sync.RenderSyncPolicy.Type = FString::Printf(TEXT("None"));
			break;
		default:
			RootCluster->Sync.RenderSyncPolicy.Type = FString::Printf(TEXT("Nvidia"));
			break;
		}
	}
}

void UUST_nDisplayConfigUtils::CompileBlueprint(UBlueprint* NewBlueprint)
{
	// Compile the construction script
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(NewBlueprint);
	FBlueprintEditorUtils::PostEditChangeBlueprintActors(NewBlueprint);
	
	FCompilerResultsLog LogResults;
	FKismetEditorUtilities::CompileBlueprint(NewBlueprint, EBlueprintCompileOptions::None, &LogResults);
	NewBlueprint->MarkPackageDirty();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/UST_EditorUtils.h"
#include "UnrealEdGlobals.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "AssetToolsModule.h"
//#include  "StaticMeshEditorSubsystem.h"
//#include "StaticMeshEditorSubsystemHelpers.h"
#include "Materials/Material.h"
#include "Factories/MaterialFactoryNew.h"
#include "RawMesh.h"
#include <StaticMeshDescription.h>
#include "MaterialDomain.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include <AssetRegistry/AssetRegistryModule.h>
#include "EngineUtils.h"
#include "Selection.h"
#include "ViewportClient.h"
#include "ScopedTransaction.h"
#include "HAL/FileManager.h"
#include "AssetViewUtils.h"
#include <Subsystems/EditorActorSubsystem.h>
#include <Actors/LEDTileStaticMeshActor.h>
#include <Actors/LEDTileActor.h>
#include <Actors/LEDScreenActor.h>
#include <Actors/LEDScreenGenerator.h>
#include <ContentBrowserModule.h>
#include "AssetViewUtils.h"
#include <EditorAssetLibrary.h>


void UUST_EditorUtils::OpenAssetWindow(UObject* Asset, bool& bOutSuccess, FString& OutInfoMsg)
{
	// Validate Asset
	if (Asset == nullptr)
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Open Asset Window Failed - Asset is not valid."));
		return;
	}

	// Get Subsystem
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor ? GEditor->GetEditorSubsystem<UAssetEditorSubsystem>() : nullptr;
	if (AssetEditorSubsystem == nullptr)
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Open Asset Window Failed - Asset Editor Subsystem is not valid."));
		return;
	}

	// Open Window
	bOutSuccess = AssetEditorSubsystem->OpenEditorForAsset(Asset);
	OutInfoMsg = FString::Printf(TEXT("Open Asset Window %s"), *FString(bOutSuccess ? "Succeeded" : "Failed"));
}

void UUST_EditorUtils::CloseAssetWindow(UObject* Asset, bool& bOutSuccess, FString& OutInfoMsg)
{
	// Validate Asset
	if (Asset == nullptr)
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Close Asset Window Failed - Asset is not valid."));
		return;
	}

	// Get Subsystem
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor ? GEditor->GetEditorSubsystem<UAssetEditorSubsystem>() : nullptr;
	if (AssetEditorSubsystem == nullptr)
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Close Asset Window Failed - Asset Editor Subsystem is not valid."));
		return;
	}

	// Close Window
	int QuantityAssetClosed = AssetEditorSubsystem->CloseAllEditorsForAsset(Asset);

	bOutSuccess = QuantityAssetClosed > 0;
	OutInfoMsg = FString::Printf(TEXT("Close Asset Window %s"), *FString(bOutSuccess ? "Succeeded" : "Failed"));
}

TArray<UObject*> UUST_EditorUtils::GetAllAssetsWithOpenedWindow(bool& bOutSuccess, FString& OutInfoMsg)
{
	// Get Subsystem
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor ? GEditor->GetEditorSubsystem<UAssetEditorSubsystem>() : nullptr;
	if (AssetEditorSubsystem == nullptr)
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Get All Assets With Opened Window Failed - Asset Editor Subsystem is not valid."));
		return TArray<UObject*>();
	}

	// Get Opened Assets
	TArray<UObject*> Assets = AssetEditorSubsystem->GetAllEditedAssets();

	bOutSuccess = true;
	OutInfoMsg = FString::Printf(TEXT("Get All Assets With Opened Window Succeeded"));
	return Assets;
}


UObject* UUST_EditorUtils::CreateAsset(FString AssetPath, UClass* AssetClass, UFactory* AssetFactory, bool& bOutSuccess, FString& OutInfoMsg)
{
	// Get the asset tools module
	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// Find right factory
	UFactory* Factory = AssetFactory;
	if (Factory == nullptr)
	{
		for (UFactory* Fac : AssetTools.GetNewAssetFactories())
		{
			if (Fac->SupportedClass == AssetClass)
			{
				Factory = Fac;
				break;
			}
		}

		if (Factory == nullptr)
		{
			bOutSuccess = false;
			OutInfoMsg = FString::Printf(TEXT("Create Asset Failed - Was not able to find factory for AssetClass. '%s'"), *AssetPath);
			return nullptr;
		}
	}

	// Can factory create the desired asset?
	if (Factory->SupportedClass != AssetClass)
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Create Asset Failed - Factory cannot create desired AssetClass. '%s'"), *AssetPath);
		return nullptr;
	}

	// Create asset
	UObject* Asset = AssetTools.CreateAsset(FPaths::GetBaseFilename(AssetPath), FPaths::GetPath(AssetPath), AssetClass, Factory);

	// Make sure it worked
	if (Asset == nullptr)
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Create Asset Failed - Either the path is invalid or the asset already exists. '%s'"), *AssetPath);
		return nullptr;
	}

	// Return the asset
	bOutSuccess = true;
	OutInfoMsg = FString::Printf(TEXT("Create Asset Succeeded - '%s'"), *AssetPath);
	return Asset;
}

UMaterial* UUST_EditorUtils::CreateMaterialAsset(FString AssetPath, bool& bOutSuccess, FString& OutInfoMsg)
{
	// Factory
	UMaterialFactoryNew* Factory = NewObject<UMaterialFactoryNew>();
	Factory->InitialTexture = nullptr;

	// Create and return asset
	UObject* Asset = CreateAsset(AssetPath, UMaterial::StaticClass(), Factory, bOutSuccess, OutInfoMsg);
	return Cast<UMaterial>(Asset);
}



FRawMesh UUST_EditorUtils::GetPlaneRawMesh(float Width, float Length, bool& bOutSuccess, FString& OutInfoMsg)
{
	FRawMesh SourceRawMesh;

	SourceRawMesh.VertexPositions.Emplace(FVector3f(0.f, 0.f, 0.f)); // Bottom left
	SourceRawMesh.VertexPositions.Emplace(FVector3f(Width, 0.0f, 0.0f)); // Bottom right
	SourceRawMesh.VertexPositions.Emplace(FVector3f(Width, 0.0f, Length)); // Top right
	SourceRawMesh.VertexPositions.Emplace(FVector3f(0.0f, 0.0f, Length)); // Top left

	// Add two triangles for the faces of the plane 
	// (Order is important for normal direction)
	SourceRawMesh.WedgeIndices.Add(0); // First triangle, bottom left
	SourceRawMesh.WedgeIndices.Add(3); // First triangle, top right
	SourceRawMesh.WedgeIndices.Add(2); // First triangle, bottom right

	SourceRawMesh.WedgeIndices.Add(0); // Second triangle, bottom left
	SourceRawMesh.WedgeIndices.Add(2); // Second triangle, top left
	SourceRawMesh.WedgeIndices.Add(1); // Second triangle, top right

	// The UV coordinates are mapped from 0 to 1 along the plane
	SourceRawMesh.WedgeTexCoords[0].Add(FVector2f(0, 0));
	SourceRawMesh.WedgeTexCoords[0].Add(FVector2f(0, 1));
	SourceRawMesh.WedgeTexCoords[0].Add(FVector2f(1, 1));


	SourceRawMesh.WedgeTexCoords[0].Add(FVector2f(0, 0));
	SourceRawMesh.WedgeTexCoords[0].Add(FVector2f(1, 1));
	SourceRawMesh.WedgeTexCoords[0].Add(FVector2f(1, 0));


	// Add normals, tangents, and UVs for each vertex
	for (int32 i = 0; i < 6; i++)
	{
		// The normal is the same for all vertices, pointing up
		SourceRawMesh.WedgeTangentZ.Add(FVector3f(0.0f, 0.0f, 1.0f));
		// The tangent and binormal are perpendicular to the normal
		SourceRawMesh.WedgeTangentX.Add(FVector3f(1.0f, 0.0f, 0.0f));
		SourceRawMesh.WedgeTangentY.Add(FVector3f(0.0f, 1.0f, 0.0f));

		SourceRawMesh.WedgeColors.Add(FColor(255, 255, 255));
	}


	// Add a material index for each face
	SourceRawMesh.FaceMaterialIndices.Add(0); // First triangle
	SourceRawMesh.FaceMaterialIndices.Add(0); // Second triangle

	SourceRawMesh.FaceSmoothingMasks.Add(1); // Phong
	SourceRawMesh.FaceSmoothingMasks.Add(1); // Phong

	if (!SourceRawMesh.IsValid())
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Raw Mesh is NotValid"));
	}
	else {
		bOutSuccess = true;
		OutInfoMsg = FString::Printf(TEXT("Raw Mesh is valid"));
	}
	return SourceRawMesh;
}

FMeshDescription UUST_EditorUtils::BuildPlaneMeshDescription()
{
	FMeshDescription mesh_desc;

	FStaticMeshAttributes attributes(mesh_desc);
	attributes.Register();

	TVertexAttributesRef<FVector3f> positions = mesh_desc.GetVertexPositions();

	mesh_desc.ReserveNewVertices(4);
	FVertexID v0 = mesh_desc.CreateVertex();
	FVertexID v1 = mesh_desc.CreateVertex();
	FVertexID v2 = mesh_desc.CreateVertex();
	FVertexID v3 = mesh_desc.CreateVertex();

	mesh_desc.ReserveNewVertexInstances(4);
	FVertexInstanceID vi0 = mesh_desc.CreateVertexInstance(v0);
	FVertexInstanceID vi1 = mesh_desc.CreateVertexInstance(v1);
	FVertexInstanceID vi2 = mesh_desc.CreateVertexInstance(v2);
	FVertexInstanceID vi3 = mesh_desc.CreateVertexInstance(v3);

	mesh_desc.ReserveNewUVs(4);
	FUVID uv0 = mesh_desc.CreateUV();
	FUVID uv1 = mesh_desc.CreateUV();
	FUVID uv2 = mesh_desc.CreateUV();
	FUVID uv3 = mesh_desc.CreateUV();

	FPolygonGroupID polygon_group = mesh_desc.CreatePolygonGroup();

	mesh_desc.ReserveNewPolygons(1);
	mesh_desc.CreatePolygon(polygon_group, { vi0, vi1, vi2, vi3 });

	positions = attributes.GetVertexPositions();

	positions[0] = FVector3f(-100, -100, 0);
	positions[1] = FVector3f(-100, 100, 0);
	positions[2] = FVector3f(100, 100, 0);
	positions[3] = FVector3f(100, -100, 0);

	TVertexInstanceAttributesRef<FVector3f> normals = attributes.GetVertexInstanceNormals();

	normals[0] = FVector3f(0, 0, 1);
	normals[1] = FVector3f(0, 0, 1);
	normals[2] = FVector3f(0, 0, 1);
	normals[3] = FVector3f(0, 0, 1);

	TVertexInstanceAttributesRef<FVector2f> uvs = attributes.GetVertexInstanceUVs();

	uvs[0] = FVector2f(0, 0);
	uvs[1] = FVector2f(0, 1);
	uvs[2] = FVector2f(1, 1);
	uvs[3] = FVector2f(1, 0);


	return mesh_desc;
}

UStaticMesh* UUST_EditorUtils::CreateStaticMeshWithDefaultMaterial(FMeshDescription& Desc, UObject* InOuter, FName InName)
{
	FStaticMeshComponentRecreateRenderStateContext RecreateRenderStateContext(FindObject<UStaticMesh>(InOuter, *InName.ToString()));
	auto StaticMesh = NewObject<UStaticMesh>(InOuter, InName, RF_Public | RF_Standalone);


	UStaticMesh::FBuildMeshDescriptionsParams mdParams;
	mdParams.bBuildSimpleCollision = true;
	mdParams.bMarkPackageDirty = true;
	mdParams.bUseHashAsGuid = false;

	// StaticMesh->NaniteSettings.bEnabled = true;

	// Build static mesh
	StaticMesh->BuildFromMeshDescriptions({ &Desc }, mdParams);

	// Materials
	TSet<UMaterialInterface*> UniqueMaterials;

	const int32 NumSections = 1;
	for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
	{
		UMaterialInterface* Material = UMaterial::GetDefaultMaterial(MD_Surface);
		UniqueMaterials.Add(Material);
	}

	for (UMaterialInterface* Material : UniqueMaterials)
	{
		// Material
		FStaticMaterial&& StaticMat = FStaticMaterial(Material);

		StaticMat.UVChannelData.bInitialized = true;
		StaticMesh->GetStaticMaterials().Add(StaticMat);

	}


	// Mesh Section Info
	FStaticMeshRenderData* const RenderData = StaticMesh->GetRenderData();

	int32 LODIndex = 0;
	int32 MaxLODs = RenderData->LODResources.Num();
	for (int32 MaterialID = 0; LODIndex < MaxLODs; ++LODIndex)
	{
		FStaticMeshLODResources& LOD = RenderData->LODResources[LODIndex];

		for (int32 SectionIndex = 0; SectionIndex < LOD.Sections.Num(); ++SectionIndex)
		{
			FStaticMeshSection& Section = LOD.Sections[SectionIndex];
			Section.MaterialIndex = MaterialID;
			Section.bEnableCollision = true;
			Section.bCastShadow = true;
			Section.bForceOpaque = false;
			MaterialID++;
		}
	}

	return StaticMesh;
}

FMeshDescription UUST_EditorUtils::BuildGridMeshDescription(int x_count, int y_count, float x_scale, float y_scale)
{
	FMeshDescription mesh_desc;

	FStaticMeshAttributes attributes(mesh_desc);
	attributes.Register();

	int vertex_count = x_count * y_count;
	int quad_count = (x_count - 1) * (y_count - 1);
	int triangle_count = quad_count * 2;

	mesh_desc.ReserveNewVertices(vertex_count);
	mesh_desc.ReserveNewVertexInstances(vertex_count);

	mesh_desc.CreatePolygonGroup();
	mesh_desc.ReserveNewPolygons(triangle_count);
	mesh_desc.ReserveNewTriangles(triangle_count);
	mesh_desc.ReserveNewEdges(triangle_count * 3);

	for (int v = 0; v < vertex_count; ++v)
	{
		mesh_desc.CreateVertex();
		mesh_desc.CreateVertexInstance(v);
	}

	for (int x = 0; x < x_count - 1; ++x)
	{
		for (int y = 0; y < y_count - 1; ++y)
		{
			FVertexInstanceID v0 = y + x * x_count;
			FVertexInstanceID v1 = v0 + 1;
			FVertexInstanceID v3 = y + (x + 1) * x_count;
			FVertexInstanceID v2 = v3 + 1;

			mesh_desc.CreateEdge(v0, v1);
			mesh_desc.CreateEdge(v1, v3);
			mesh_desc.CreateEdge(v3, v0);

			mesh_desc.CreateEdge(v1, v2);
			mesh_desc.CreateEdge(v2, v3);
			mesh_desc.CreateEdge(v3, v1);
		}
	}

	mesh_desc.BuildVertexIndexers();

	for (int x = 0; x < x_count - 1; ++x)
	{
		for (int y = 0; y < y_count - 1; ++y)
		{
			FVertexInstanceID v0 = y + x * x_count;
			FVertexInstanceID v1 = v0 + 1;
			FVertexInstanceID v3 = y + (x + 1) * x_count;
			FVertexInstanceID v2 = v3 + 1;

			mesh_desc.CreateTriangle(0, { v0, v1, v3 });
			mesh_desc.CreateTriangle(0, { v1, v2, v3 });
		}
	}

	auto positions = mesh_desc.GetVertexPositions().GetRawArray();;
	auto uvs = mesh_desc.VertexInstanceAttributes().GetAttributesRef<FVector2f>(MeshAttribute::VertexInstance::TextureCoordinate).GetRawArray();

	for (int x = 0; x < x_count; ++x)
	{
		for (int y = 0; y < y_count; ++y)
		{
			int i = y + x * x_count;

			float u = (float)y / (y_count - 1);
			float v = (float)x / (x_count - 1);

			uvs[i] = FVector2f(u, v);
			positions[i] = FVector3f((v - 0.5f) * x_scale, (u - 0.5f) * y_scale, 0);
		}
	}

	return mesh_desc;
}

void UUST_EditorUtils::ShowEditorNotification(FString Message)
{
	FNotificationInfo Info(FText::FromString(Message));
	Info.ExpireDuration = 5.0f;
	//Info.FadeInDuration = 0.5f;
	//Info.FadeOutDuration = 0.5f;
	//Info.WidthOverride = 400.0f;
	FSlateNotificationManager::Get().AddNotification(Info);
}

bool UUST_EditorUtils::RemoveActorInWorldOutliner(FString Name)
{
	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();

	AActor* ActorFound = GetActorWithLabel(Name);

	TArray<AActor*> AttachedActors;

	if (IsValid(ActorFound)) {

		ActorFound->GetAttachedActors(AttachedActors, true, true);

		for (auto actor : AttachedActors)
		{
			//actor->D
			currentWorld->EditorDestroyActor(actor, false);
		}
		currentWorld->EditorDestroyActor(ActorFound, true);

		return true;
	}

	return false;
}

AActor* UUST_EditorUtils::GetActorWithLabel(FString Name)
{
	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();

	bool bFoundActorWithGivenName = false;
	AActor* ActorFound = nullptr;

	TActorIterator<AActor> ActorItr(currentWorld);
	while (ActorItr) {

		if (ActorItr->GetActorLabel() == Name)
		{
			ActorFound = *ActorItr;
			bFoundActorWithGivenName = true;
			break;
		}
		++ActorItr;
	}


	if (bFoundActorWithGivenName)
		return ActorFound;

	return nullptr;
}

EAppReturnType::Type UUST_EditorUtils::EditorMessageDialog(EAppMsgType::Type MessageType, const FString Message, const FString OptTitle)
{
	FText const Title = FText::FromString(*OptTitle);
	FText const DialogText = FText::FromString(*Message);
	EAppReturnType::Type const ReturnType = FMessageDialog::Open(MessageType, DialogText, &Title);
	return ReturnType;
}

void UUST_EditorUtils::RemoveAssetAtPath(FString Path, FString AssetToDelete)
{
	// Get the AssetRegistry
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FString FindPath = Path;
	FindPath.Append(TEXT("/"));

	// Scan the Path
	TArray<FString> PathsToScan;
	PathsToScan.Add(FindPath);
	AssetRegistry.ScanPathsSynchronous(PathsToScan);

	// Retrive Assets by Path
	TArray<FAssetData> MeshAssetList;
	AssetRegistry.GetAssetsByPath(FName(*FindPath), MeshAssetList);

	// Deal with Asset Data
	for (const FAssetData& AssetData : MeshAssetList)
	{
		if (AssetData.AssetName.ToString() == AssetToDelete)
		{
			UStaticMesh* MyMesh = Cast<UStaticMesh>(AssetData.GetAsset());
			if (MyMesh)
			{
				MyMesh->MarkPendingKill();

				// ForceGarbageCollection approach can lead to crashes, especially if some assets are already set to nullptr
				//GEditor->ForceGarbageCollection(true);

				FAssetRegistryModule::AssetDeleted(AssetData.GetAsset());
			}
		}
	}
}

void UUST_EditorUtils::RemoveSelectedActors() {
	// Provide a context and description for the undo session
	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();

	GEditor->BeginTransaction(FText::FromString("Delete Selected Actors"));

	if (USelection* SelectedActors = GEditor->GetSelectedActors()) {
		for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter) {
			ALEDTileStaticMeshActor* Actor = Cast<ALEDTileStaticMeshActor>(*Iter);
			if (IsValid(Actor)) {
				//Actor->Modify();
				//Actor->ConditionalBeginDestroy();
				//Actor->Destroy();
				currentWorld->EditorDestroyActor(Actor, true);
			}
			else if (ALEDTileActor* LEDTileActor = Cast<ALEDTileActor>(*Iter)) {
				DestroyAllAttachedActors(LEDTileActor);
				currentWorld->EditorDestroyActor(LEDTileActor, true);
			}
			else if (ALEDScreenActor* LEDScreenActor = Cast<ALEDScreenActor>(*Iter)) {
				DestroyAllAttachedActors(LEDScreenActor);
				currentWorld->EditorDestroyActor(LEDScreenActor, true);
			}
			else if (ALEDScreenGenerator* LEDScreenGenActor = Cast<ALEDScreenGenerator>(*Iter)) {
				DestroyAllAttachedActors(LEDScreenGenActor);
				currentWorld->EditorDestroyActor(LEDScreenGenActor, true);
			}
			else {
				UUST_EditorUtils::EditorMessageDialog(EAppMsgType::Ok, FString::Printf(TEXT("Only LEDScreenGenerator or LEDScreenActor or LEDTileActor or LEDStaticMeshActor can be removed.")), TEXT("UBuild - Edit Section"));
				//UUST_EditorUtils::ShowEditorNotification(TEXT("Only LEDStaticMeshActor can be removed."));
			}
		}
	}

	//UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>(); 

	//if (!EditorActorSubsystem)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Editor Actor Subsystem not found!"));
	// GEditor->SelectActor(Instance, true, true, true);
	//	EditorActorSubsystem->DestroyActor(Actor);
	//	GEngine->ForceGarbageCollection(true);
	//}

	GEditor->EndTransaction();
}

void UUST_EditorUtils::DestroyAllAttachedActors(AActor* ParentActor, bool bRecursiveActors)
{
	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();

	TArray<AActor*> AttachedActors;
	ParentActor->GetAttachedActors(AttachedActors, true, bRecursiveActors);
	for (auto* Attached : AttachedActors)
	{
		//Attached->Destroy();
		// 
		//call Rename() without any arguments on the old object before overwriting it.
		//Unreal  Engine will then call MakeUniqueObjectName and give some random name which will free up the name you want to use.
		//Attached->Rename();
		//currentWorld->BeginDestroy();
		currentWorld->EditorDestroyActor(Attached, true);
		//currentWorld->RemoveActor(Attached,true);
		//GEditor->BeginDestroy
		//FAssetRegistryModule::AssetDeleted(Attached);
		
	}
}


void UUST_EditorUtils::SelectActorInWorldOutliner(AActor* ActorToSelect)
{
	if (IsValid(ActorToSelect)) {
		GEditor->SelectActor(ActorToSelect, true, true);
	}
}

void UUST_EditorUtils::CreateFolder(FString FolderPath, bool& bOutSuccess, FString& OutInfoMsg)
{
	// Convert project path to absolute
	FString AbsoluteFolderPath;
	if (!FPackageName::TryConvertFilenameToLongPackageName(FolderPath, AbsoluteFolderPath)) {
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Create Folder Failed - Failed to convert Folder Path to absolute %s"), *AbsoluteFolderPath);
		return;
	}

	// Create Folder
	if (!IFileManager::Get().MakeDirectory(*AbsoluteFolderPath, true)) {
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Create Folder Failed - Failed to create Folder.  '%s'"), *AbsoluteFolderPath);
		return;
	}

	bOutSuccess = true;
	OutInfoMsg = FString::Printf(TEXT("Create Folder Succeeded -  '%s'"), *AbsoluteFolderPath);

}

void UUST_EditorUtils::DeleteFolder(FString FolderPath, bool& bOutSuccess, FString& OutInfoMsg)
{
	// Delete Folder
	if (!AssetViewUtils::DeleteFolders({ FolderPath })) {
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Delete Folder Failed - Failed to Delete Folder %s"), *FolderPath);
		return;
	}

	bOutSuccess = true;
	OutInfoMsg = FString::Printf(TEXT("Delete Folder Succeeded - '%s'"), *FolderPath);

}

void UUST_EditorUtils::IsFolderExists(FString FolderPath, bool& bOutSuccess, FString& OutInfoMsg)
{
	if (!AssetViewUtils::DoesFolderExist(FolderPath)) {
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Folder does not exists at path %s"), *FolderPath);
		return;
	}

	bOutSuccess = true;
	OutInfoMsg = FString::Printf(TEXT("Folder exists at path %s"), *FolderPath);
}

void UUST_EditorUtils::RenameActor(AActor* ActorToRename, FString NewName) {
	if (IsValid(ActorToRename))
	{
		ActorToRename->SetActorLabel(*NewName);

	}
}

void UUST_EditorUtils::AttachActorToParentActor(AActor* Actor, AActor* ParentActor) {
	if (IsValid(Actor) && IsValid(ParentActor))
	{
		Actor->GetRootComponent()->SetMobility(EComponentMobility::Movable);

		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::KeepWorld, // Location rule
			EAttachmentRule::KeepRelative, // Rotation rule
			EAttachmentRule::KeepWorld, // Scale rule
			false // Weld simulated bodies
		);

		// Attach ChildActor to ParentActor
		Actor->AttachToActor(ParentActor, AttachmentRules);
	}
}

AActor* UUST_EditorUtils::SpawnEmptyActor(FTransform Transform, FString Name, UClass* actorClass)
{
	FTransform objectTrasform(Transform);

	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();
	
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

AActor* UUST_EditorUtils::SpawnActorFromClass(FTransform Transform, FString Name, UClass* actorClass)
{
	FTransform objectTrasform(Transform);
	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();
	
	AActor* newActorCreated = GEditor->AddActor(currentLevel, actorClass, objectTrasform, true, RF_Public | RF_Standalone | RF_Transactional);
	newActorCreated->SetActorLabel(*Name);
	return newActorCreated;
}

UActorComponent* UUST_EditorUtils::AddComponentToActor(AActor* actor, TSubclassOf<UActorComponent> ComponentClass, FString ComponentName)
{
	if(!actor)
		return nullptr;

	UWorld* currentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* currentLevel = currentWorld->GetCurrentLevel();
	
	UActorComponent* ComponentRoot = NewObject<UActorComponent>(actor, *ComponentClass,*ComponentName, RF_Transactional);
	
	if (!ComponentRoot)
		return nullptr;
	
	actor->AddInstanceComponent(ComponentRoot);
	ComponentRoot->RegisterComponent();
	
	GEditor->EditorUpdateComponents();
	ComponentRoot->RegisterComponentWithWorld(currentWorld);
	currentWorld->UpdateWorldComponents(true, false);
	return ComponentRoot;
}

void UUST_EditorUtils::DeleteFilesInFolder(const FString& FolderPath)
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	IContentBrowserSingleton& ContentBrowserSingleton = ContentBrowserModule.Get();
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByPath(*FolderPath, AssetData, true);
	for (const FAssetData& Asset : AssetData)
	{
		//UObject* Obj = Asset.GetAsset();
		FString AssetName = Asset.AssetName.ToString();
		FString AssetPackagePath = Asset.PackagePath.ToString();
		FString AssetPath = Asset.GetSoftObjectPath().ToString();
	
		UE_LOG(LogTemp, Warning, TEXT("Asset Name: %s, PackagePath: %s "), *AssetName, *AssetPackagePath);
		//RemoveAssetAtPath(AssetPackagePath, AssetName);
		// Delete the asset using EditorAssetLibrary
		if (UEditorAssetLibrary::DeleteAsset(AssetPath))
		{
			// Successfully deleted
			UE_LOG(LogTemp, Warning, TEXT("Deleted asset: %s"), *AssetPath);
		}
		else
		{
			// Handle deletion failure (optional)
			UE_LOG(LogTemp, Error, TEXT("Failed to delete asset: %s"), *AssetPath);
		}
	}
	
	
	
}

bool UUST_EditorUtils::DeleteDirectoryRecursively(const FString& DirectoryPath)
{
	// Get the platform file manager
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Check if directory exists
	if (!PlatformFile.DirectoryExists(*DirectoryPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Directory '%s' doesn't exist"), *DirectoryPath);
		return false;
	}

	// Iterate over directory contents
	TArray<FString> DirectoriesToDelete;
	PlatformFile.IterateDirectory(*DirectoryPath, [&,DirectoryPath](const TCHAR* FilenameOrDirectory, bool bIsDirectory) -> bool
		{
			// Skip current and parent directories
			if (FCString::Strcmp(FilenameOrDirectory, TEXT("..")) == 0)
			{
				return true;
			}

			// Build full path
			FString FullPath = FString::Printf(TEXT("%s/%s"), *DirectoryPath, FilenameOrDirectory);

			if (bIsDirectory)
			{
				// Recursively delete subdirectories
				if (!UUST_EditorUtils::DeleteDirectoryRecursively(FullPath))
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to delete directory '%s' recursively"), *FullPath);
					return false; // Stop iteration if deletion fails
				}
				// Add the directory to the list for deletion
				DirectoriesToDelete.Add(FullPath);
			}
			else
			{
				// Add the file to the list for deletion
				PlatformFile.DeleteFile(*FullPath);
			}

			return true;
		});

	// Delete the directories
	bool bSuccess = true;
	for (const FString& DirectoryToDelete : DirectoriesToDelete)
	{
		if (!PlatformFile.DeleteDirectory(*DirectoryToDelete))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to delete directory '%s'"), *DirectoryToDelete);
			bSuccess = false;
		}
	}

	// Finally, delete the original directory
	if (!PlatformFile.DeleteDirectory(*DirectoryPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to delete directory '%s'"), *DirectoryPath);
		bSuccess = false;
	}

	return bSuccess;
}

void UUST_EditorUtils::LaunchProcess(FString FilePath,bool LaunchDetached)
{
	if (FPaths::FileExists(FilePath))
	{
		auto ProcHandle = FPlatformProcess::CreateProc(*FilePath, TEXT(""), LaunchDetached, false, false, nullptr, 0, nullptr, nullptr);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Unable to find batch file at %s."),*FilePath);
	}
}






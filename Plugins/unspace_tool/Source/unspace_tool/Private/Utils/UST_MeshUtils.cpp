// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/UST_MeshUtils.h"
#include "Factories/Factory.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include <Subsystems/EditorAssetSubsystem.h>
#include <RawMesh.h>

#include "ContentBrowserModule.h"
#include "Factories/StaticMeshAssetFactory.h"
#include "Utils/UST_MathUtils.h"


// TArray<FVector2f>& UV0, TArray<FVector2f>& UV1, TArray<FVector2f>& UV2, TArray<FVector2f>& UV3,
void UUST_MeshUtils::GetStaticMeshInfo(const FTransform& ScreenTransform, UStaticMeshComponent* InMeshComponent, TArray<int32>& Triangles, TArray<FVector3f>& MeshVerts, TArray<FVector3f>& UVVerts,
		 TArray<FVector3f>& Normals, TArray<FVector3f>& Tangents, TArray<FVector3f>& Binormals,  TArray<FColor>& VertexColors)
{
	UStaticMesh* InMesh = InMeshComponent->GetStaticMesh();

	FTransform ComponentTransform = InMeshComponent->GetComponentTransform();

    FTransform RelTransform = UUST_MathUtils::CalculateRelativeTransform(ComponentTransform, ScreenTransform);
    UE_LOG(LogTemp, Warning, TEXT("RelTransform %s"), *RelTransform.ToString());

	if (InMesh->GetRenderData()->LODResources.Num() > 0) {
		FStaticMeshVertexBuffers* VertexBuffers = &(InMesh->GetRenderData()->LODResources[0].VertexBuffers);
		FPositionVertexBuffer* PositionVertexBuffer = &(VertexBuffers->PositionVertexBuffer);
		FColorVertexBuffer* ColorBuffer = &(VertexBuffers->ColorVertexBuffer);
		FStaticMeshVertexBuffer* SMVertexBuffer = &(VertexBuffers->StaticMeshVertexBuffer);

		FIndexArrayView IndexBuffer = InMesh->GetRenderData()->LODResources[0].IndexBuffer.GetArrayView();

		ColorBuffer->GetVertexColors(VertexColors);


		if (PositionVertexBuffer) {

			const int32 VertexCount = PositionVertexBuffer->GetNumVertices();

			for (int32 vertIndex = 0; vertIndex < VertexCount; vertIndex++) {
				const FVector3f Vertex = PositionVertexBuffer->VertexPosition(vertIndex);

                UE_LOG(LogTemp, Warning, TEXT("Vertex %s"), *Vertex.ToString());

				const FVector3f Tangent = SMVertexBuffer->VertexTangentX(vertIndex);
				const FVector3f Binormal = SMVertexBuffer->VertexTangentY(vertIndex);
				const FVector3f Normal = SMVertexBuffer->VertexTangentZ(vertIndex);

				/*const FVector2f LocalUV0 = SMVertexBuffer->GetVertexUV(vertIndex,0);
				const FVector2f LocalUV1 = SMVertexBuffer->GetVertexUV(vertIndex, 1);
				const FVector2f LocalUV2 = SMVertexBuffer->GetVertexUV(vertIndex, 2);
				const FVector2f LocalUV3 = SMVertexBuffer->GetVertexUV(vertIndex, 3);*/

				// Transform vertices and vertex normals from local space to world space
				/*ComponentTransform.TransformPosition(Vertex);
				ComponentTransform.TransformVector(Normal);*/

				// Add the vertex to an output FVector3f array (e.g., MeshVerts)
				ComponentTransform.TransformPosition(StaticCast<FVector>(Vertex));

                const FVector3f TransformedVertex = StaticCast<FVector3f>(RelTransform.TransformPosition(StaticCast<FVector>(Vertex)));
                //UE_LOG(LogTemp, Warning, TEXT("TransformedVertex %s"), *TransformedVertex.ToString());
                //const FVector3f RelTransformVertex = UUST_MathUtils::ConvertToFVector3f(ScreenTransform.GetLocation()) - TransformedVertex;
                //UE_LOG(LogTemp, Warning, TEXT("FinalTransformVertex %s"), *FinalTransformVertex.ToString());
				UVVerts.Add(TransformedVertex);
				MeshVerts.Add(StaticCast<FVector3f>(ComponentTransform.TransformPosition(StaticCast<FVector>(Vertex))));
				Normals.Add(StaticCast<FVector3f>(RelTransform.TransformVector(StaticCast<FVector>(Normal))));
				Tangents.Add(StaticCast<FVector3f>(RelTransform.TransformVector(StaticCast<FVector>(Tangent))));
				Binormals.Add(StaticCast<FVector3f>(RelTransform.TransformVector(StaticCast<FVector>(Binormal))));

			/*	UV0.Add(LocalUV0);
				UV1.Add(LocalUV1);
				UV2.Add(LocalUV2);
				UV3.Add(LocalUV3);*/
			}

		}
	}
}


void UUST_MeshUtils::ConvertQuadToTriangles(TArray<int32>& Triangles, TArray<int32>& MaterialIndices, int32 Vert0, int32 Vert1, int32 Vert2, int32 Vert3, int32 NewMaterialGroup)
{
	Triangles.Add(Vert0);
	Triangles.Add(Vert1);
	Triangles.Add(Vert3);

	Triangles.Add(Vert1);
	Triangles.Add(Vert2);
	Triangles.Add(Vert3);

	if (NewMaterialGroup != INDEX_NONE)
	{
		MaterialIndices.Add(NewMaterialGroup);

		MaterialIndices.Add(NewMaterialGroup);
	}
}

void UUST_MeshUtils::GetMeshSectionInfo(UStaticMeshComponent* StaticMeshComponent)
{
	//StaticMeshComponent->GetMeshSe
		
		//GetMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

UStaticMesh* UUST_MeshUtils::CreateStaticMesh(UObject* InParent, FName Name, EObjectFlags Flags)
{
	UStaticMeshAssetFactory* Factory = NewObject<UStaticMeshAssetFactory>();

	UObject* newObject = Factory->CreateOrOverwriteAsset(UStaticMesh::StaticClass(), InParent, Name, Flags);
	UStaticMesh* newStaticMesh = nullptr;
	if (newObject)
	{
		newStaticMesh = CastChecked<UStaticMesh>(newObject);
	}

	return newStaticMesh;	
}

UStaticMesh* UUST_MeshUtils::GetStaticMeshIfExists(FString AssetPath, bool& bOutSuccess, FString& OutInfoMsg)
{
	UEditorAssetSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!IsValid(EditorAssetSubsystem))
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("EditorAssetSubsystem is InValid"), *AssetPath);

		return nullptr;
	}


	if(!EditorAssetSubsystem->DoesAssetExist(AssetPath))
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Asset doesnt exists at path %s"), *AssetPath);

		return nullptr;
	}

	
	UObject* Obj = EditorAssetSubsystem->LoadAsset(AssetPath);
	UStaticMesh* newStaticMesh = Cast<UStaticMesh>(Obj);
	if (!IsValid(newStaticMesh))
	{
		bOutSuccess = false;
		OutInfoMsg = FString::Printf(TEXT("Asset is not static mesh at path %s"), *AssetPath);
		return nullptr;
	}

	bOutSuccess = true;
	OutInfoMsg = FString::Printf(TEXT("Successfully found Static Mesh at path %s"), *AssetPath);
	return newStaticMesh;
}

UStaticMesh* UUST_MeshUtils::ConvertProceduralMeshToStaticMesh(UProceduralMeshComponent* ProcMesh, FString AssetPath, FString AssetName) {
    //UStaticMesh = UStaticMesh::create

    // Find first selected ProcMeshComp
    UProceduralMeshComponent* ProcMeshComp = ProcMesh;
    if (ProcMeshComp != nullptr)
    {
        //FString ActorName = ProcMesh->GetOwner()->GetName();
        //FString LevelName = ProcMesh->GetWorld()->GetMapName();
        //FString AssetName = FString(TEXT("SM_")) + LevelName + FString(TEXT("_") + ActorName);
        //FString PathName = FString(TEXT("/Game/"));
        FString PackageName = AssetPath + AssetName;

        // Raw mesh data we are filling in
        FRawMesh RawMesh;
        // Materials to apply to new mesh
        TArray<UMaterialInterface*> MeshMaterials;

        const int32 NumSections = ProcMeshComp->GetNumSections();
        int32 VertexBase = 0;
        for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++) {
            FProcMeshSection* ProcSection = ProcMeshComp->GetProcMeshSection(SectionIdx);

            // Copy verts
            for (FProcMeshVertex& Vert : ProcSection->ProcVertexBuffer) {
                RawMesh.VertexPositions.Add(FVector3f(Vert.Position));
            }

            // Copy 'wedge' info
            int32 NumIndices = ProcSection->ProcIndexBuffer.Num();
            for (int32 IndexIdx = 0; IndexIdx < NumIndices; IndexIdx++) {
                int32 Index = ProcSection->ProcIndexBuffer[IndexIdx];

                RawMesh.WedgeIndices.Add(Index + VertexBase);

                FProcMeshVertex& ProcVertex = ProcSection->ProcVertexBuffer[Index];

                FVector3f TangentX = FVector3f(ProcVertex.Tangent.TangentX);
                FVector3f TangentZ = FVector3f(ProcVertex.Normal);
                FVector3f TangentY = FVector3f((TangentX ^ TangentZ).GetSafeNormal() * (ProcVertex.Tangent.bFlipTangentY ? -1.f : 1.f));

                RawMesh.WedgeTangentX.Add(TangentX);
                RawMesh.WedgeTangentY.Add(TangentY);
                RawMesh.WedgeTangentZ.Add(TangentZ);

                RawMesh.WedgeTexCoords[0].Add(FVector2f(ProcVertex.UV0));
                RawMesh.WedgeColors.Add(ProcVertex.Color);
            }

            // copy face info
            int32 NumTris = NumIndices / 3;
            for (int32 TriIdx = 0; TriIdx < NumTris; TriIdx++) {
                RawMesh.FaceMaterialIndices.Add(SectionIdx);
                RawMesh.FaceSmoothingMasks.Add(0); // Assume this is ignored as bRecomputeNormals is false
            }

            // Remember material
            MeshMaterials.Add(ProcMeshComp->GetMaterial(SectionIdx));

            // Update offset for creating one big index/vertex buffer
            VertexBase += ProcSection->ProcVertexBuffer.Num();

            // If we got some valid data.
            if (RawMesh.VertexPositions.Num() > 3 && RawMesh.WedgeIndices.Num() > 3) {
                // Then find/create it.
                UPackage* Package = CreatePackage(*PackageName);
                check(Package);

                // Create StaticMesh object
                UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, FName(*AssetName), RF_Public | RF_Standalone);
                StaticMesh->InitResources();

                FGuid::NewGuid() = StaticMesh->GetLightingGuid();
                //StaticMesh->GetLightingGuid() = FGuid::NewGuid();

                // Add source to new StaticMesh
                FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
                //FStaticMeshSourceModel* SrcModel = new (StaticMesh->SourceModels) FStaticMeshSourceModel();
                SrcModel.BuildSettings.bRecomputeNormals = true;
                SrcModel.BuildSettings.bRecomputeTangents = true;
                SrcModel.BuildSettings.bRemoveDegenerates = false;
                SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = true;
                SrcModel.BuildSettings.bUseFullPrecisionUVs = true;
                SrcModel.BuildSettings.bGenerateLightmapUVs = true;
                SrcModel.BuildSettings.SrcLightmapIndex = 0;
                SrcModel.BuildSettings.DstLightmapIndex = 1;
                SrcModel.SaveRawMesh(RawMesh);
                //SrcModel.RawMeshBulkData->SaveRawMesh(RawMesh);

                // Copy materials to new mesh
                for (UMaterialInterface* Material : MeshMaterials)
                {
                    StaticMesh->GetStaticMaterials().Add(FStaticMaterial(Material));
                }

                //Set the Imported version before calling the build
                StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;

                // Build mesh from source
                StaticMesh->Build(false);
                StaticMesh->SetLightMapResolution(256);
                StaticMesh->SetLightMapCoordinateIndex(1);

                StaticMesh->PostEditChange();

                // Notify asset registry of new asset
                FAssetRegistryModule::AssetCreated(StaticMesh);

                return StaticMesh;
            }
            else return nullptr;
        }
        return nullptr;
    }
    else {
        return nullptr;
    }
}

TArray<UStaticMesh*> UUST_MeshUtils::GetStaticMeshesInFolder(const FString& FolderPath)
{
	TArray<UStaticMesh*> StaticMeshes;
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	IContentBrowserSingleton& ContentBrowserSingleton = ContentBrowserModule.Get();
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByPath(*FolderPath, AssetData, true);
	for (const FAssetData& Asset : AssetData)
	{
		UObject* Obj = Asset.GetAsset();
		UStaticMesh* newStaticMesh = Cast<UStaticMesh>(Obj);
		StaticMeshes.Add(newStaticMesh);
	}
	SortStaticMeshes(StaticMeshes);
	return StaticMeshes;
}

void UUST_MeshUtils::SortStaticMeshes(TArray<UStaticMesh*>& StaticMeshArray)
{
	StaticMeshArray.Sort([](UStaticMesh& A, UStaticMesh& B) {
	   return CompareStaticMeshes(&A, &B);
   });
}

bool UUST_MeshUtils::CompareStaticMeshes(const UStaticMesh* A, const UStaticMesh* B)
{
	FString AName = A->GetName();
	FString BName = B->GetName();
	int32 AIndex,BIndex;
	AName.FindLastChar('_', /*out*/ AIndex) ;
	BName.FindLastChar('_', /*out*/ BIndex);
	AIndex = AIndex+1;
	BIndex = BIndex+1;
	// Get the integer value from the names
	int32 AValue = FCString::Atoi(&AName[AIndex]);
	int32 BValue = FCString::Atoi(&BName[BIndex]);
	// Compare the integer values
	return AValue < BValue;
}

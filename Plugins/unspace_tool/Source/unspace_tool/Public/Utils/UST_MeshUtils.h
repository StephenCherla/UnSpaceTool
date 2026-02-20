// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h"
#include "UST_MeshUtils.generated.h"

/**
 * 
 */
UCLASS()
class UNSPACE_TOOL_API UUST_MeshUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// TArray<FVector2f>& UV0,TArray<FVector2f>& UV1, TArray<FVector2f>& UV2, TArray<FVector2f>& UV3,
	static void GetStaticMeshInfo(const FTransform& ScreenTransform, UStaticMeshComponent* InMeshComponent, TArray<int32>& Triangles, TArray<FVector3f>& MeshVerts, TArray<FVector3f>& UVVerts,
		 TArray<FVector3f>& Normals, TArray<FVector3f>& Tangents, TArray<FVector3f>& Binormals,  TArray<FColor>& VertexColors);

	static void ConvertQuadToTriangles(TArray<int32>& Triangles, TArray<int32>& MaterialIndices, int32 Vert0, int32 Vert1, int32 Vert2, int32 Vert3, int32 NewMaterialGroup);


	static void GetMeshSectionInfo(UStaticMeshComponent* StaticMeshComponent);

	static UStaticMesh* CreateStaticMesh(UObject* InParent, FName Name, EObjectFlags Flags);

	static UStaticMesh* GetStaticMeshIfExists(FString AssetPath, bool& bOutSuccess, FString& OutInfoMsg);

	static UStaticMesh* ConvertProceduralMeshToStaticMesh(UProceduralMeshComponent* ProcMesh, FString AssetPath, FString AssetName);

	UFUNCTION(BlueprintCallable, Category = "UnSpaceTool")
	static TArray<UStaticMesh*> GetStaticMeshesInFolder(const FString& FolderPath);
	static void SortStaticMeshes(TArray<UStaticMesh*>& StaticMeshArray);
	static bool CompareStaticMeshes(const UStaticMesh* A, const UStaticMesh* B);
};

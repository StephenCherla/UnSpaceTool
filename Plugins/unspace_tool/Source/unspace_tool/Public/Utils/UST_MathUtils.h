// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UST_MathUtils.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNSPACE_TOOL_API UUST_MathUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static int32 Get1DIndexFrom2DArrayIndices(int32 TotalCols, int32 RowIndex, int32 ColIndex);

	UFUNCTION(BlueprintCallable, Category = "UST|Math")
	static TArray<FVector2f> GetPlaneGridUVs(int32 TotalRows, int32 TotalCols);

	UFUNCTION(BlueprintCallable, Category = "UST|Math")
	static TArray<int32> GetPlaneGridTriangles(int32 TotalRows, int32 TotalCols);

	UFUNCTION(BlueprintCallable, Category = "UST|Math")
	static TArray<FVector3f> GetPlaneGridNormals(int32 TotalRows, int32 TotalCols);

	UFUNCTION(BlueprintCallable, Category = "UST|Math")
	static TArray<FVector3f> GetPlaneGridTangents(int32 TotalRows, int32 TotalCols);

	UFUNCTION(BlueprintCallable, Category = "UST|Math")
	static TArray<FVector3f> GetPlaneGridBiNormals(int32 TotalRows, int32 TotalCols);




	UFUNCTION(BlueprintCallable, Category = "UST|Math")
	static TArray<FVector2f> GetUVsOfVertices(int32 TotalRows, int32 TotalCols,int32 SizeX, int32 SizeY);

	UFUNCTION(BlueprintCallable, Category = "UST|Math")
	static TArray<FVector3f> GetPositionsOfVertices();

	UFUNCTION(BlueprintCallable, Category = "UST|Math")
	static FTransform CalculateTransformOfNextTile(FTransform ActorTransform, float RotX, float RotY, float RotZ, FVector ForwardVector, float Magnitude);



	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static bool CheckTextIsNumeric(FText Text);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static float InchesToCentiMeters(float InInches);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static float CentiMetersToInches(float InCentimeters);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static float Diagonal_InInches_FromCentiMeters(float Width_cms, float Height_cms);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static float Diagonal_InPixels_FromPixels(float Horizontal_Pixels, float Vertical_Pixels);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static float CalculatePPI(float Diagonal_Pixels, float Diagonal_Inches);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static float ConvertPixelsToCentiMeters(float Pixels, float PPI = 96.0f);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static float ConvertCentiMetersToPixels(float Centimeters, float PPI = 96.0f);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static float MapFloatRange(float value, float inMin, float inMax, float outMin, float outMax);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static FVector3f ConvertToFVector3f(FVector DoublePrecisionVector);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static FTransform CalculateRelativeTransform(const FTransform& TransformA, const FTransform& TransformB);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static void GenerateMeshCoordinates(float TileWidth, float TileHeight, int32 TileNumX, int32 TileNumY, int32 ScreenNumX, int32 ScreenNumY, float OffsetX, float OffsetY,float TileAngle,FVector StartingPosition,AActor* PositionActor,TArray<FTransform>& MeshCoordinates);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	static int32 Get1dFrom2D(int32 X, int32 Y ,int32 RowCount);

	UFUNCTION(BlueprintPure, Category = "UST|Math")
	float CalculateDistanceForFullFOV(float ObjectLengthOrHeight, float FOVAngle);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/UST_MathUtils.h"
#include <Kismet/KismetMathLibrary.h>

int32 UUST_MathUtils::Get1DIndexFrom2DArrayIndices(int32 TotalCols, int32 RowIndex, int32 ColIndex)
{
	return (ColIndex + (RowIndex * TotalCols));
}

TArray<FVector2f> UUST_MathUtils::GetPlaneGridUVs(int32 TotalRows, int32 TotalCols)
{
	TArray<FVector2f> InTexCoords;

	int32 Quad_Count = (TotalRows * TotalCols);
	int32 Triangle_Count = Quad_Count * 2;

	// Per UV of Per Unit
	float u_unit_val = 1 / TotalCols; // u Value per unit (Horizontal)
	float v_unit_val = 1 / TotalRows; // v Value per unit (Vertical)

	for (int32 rowIndex = 0; rowIndex < TotalRows; rowIndex++) {
		for (int32 colIndex = 0; colIndex < TotalCols; colIndex++) {
			FVector2f uv_top_left;
			FVector2f uv_top_right;
			FVector2f uv_bottom_left;
			FVector2f uv_bottom_right;

			uv_top_left = FVector2f((colIndex * u_unit_val), (rowIndex * v_unit_val));  
			uv_bottom_left = FVector2f((colIndex * u_unit_val), (v_unit_val + (rowIndex * v_unit_val)));
			uv_bottom_right = FVector2f((u_unit_val +(colIndex * u_unit_val)), (v_unit_val + (rowIndex * v_unit_val)));
			uv_top_right = FVector2f((u_unit_val + (colIndex * u_unit_val)), ((rowIndex * v_unit_val)));

			//int32 index = Get1DIndexFrom2DArrayIndices(TotalCols, rowIndex, colIndex);


			// Plane has 2 Triangles. UVs for Each Triangle
			// UVs of 1st Triangle in Plane
			InTexCoords.Add(uv_top_left);
			InTexCoords.Add(uv_bottom_left);
			InTexCoords.Add(uv_bottom_right);


			// UVs of 2nd Traingle in Plane
			InTexCoords.Add(uv_top_left);
			InTexCoords.Add(uv_bottom_right);
			InTexCoords.Add(uv_top_right);

		}
	}
	return InTexCoords;
}

TArray<int32> UUST_MathUtils::GetPlaneGridTriangles(int32 TotalRows, int32 TotalCols)
{
	TArray<int32> InTriangles;
	InTriangles.Reserve(TotalRows * TotalCols * 6);

	int32 Index0 = 0;
	int32 Index1 = 1;
	int32 Index2 = 2;
	int32 Index3 = 3;

	//int32 CurrentIndex = Get1DIndexFrom2DArrayIndices(TotalCols, rowIndex, colIndex);
	int32 CurrentIndex = 0;

	for (int32 rowIndex = 0; rowIndex < TotalRows; rowIndex++) {
		for (int32 colIndex = 0; colIndex < TotalCols; colIndex++) {
			int32 TriangleIndex = CurrentIndex;

			InTriangles[TriangleIndex++] = Index0;
			InTriangles[TriangleIndex++] = Index1;
			InTriangles[TriangleIndex++] = Index2;

			InTriangles[TriangleIndex++] = Index0;
			InTriangles[TriangleIndex++] = Index2;
			InTriangles[TriangleIndex++] = Index3;

			CurrentIndex++;
		}
	}

	return InTriangles;
}

TArray<FVector3f> UUST_MathUtils::GetPlaneGridNormals(int32 TotalRows, int32 TotalCols)
{
	TArray<FVector3f> InNormals;
	InNormals.Reserve(TotalRows * TotalCols * 6);

	for (int32 rowIndex = 0; rowIndex < TotalRows; rowIndex++) {
		for (int32 colIndex = 0; colIndex < TotalCols; colIndex++) {
			InNormals.Add(FVector3f(0.0f, 0.0f, 1.0f));
		}
	}

	return InNormals;
}

TArray<FVector3f> UUST_MathUtils::GetPlaneGridTangents(int32 TotalRows, int32 TotalCols)
{
	TArray<FVector3f> InTangents;
	InTangents.Reserve(TotalRows * TotalCols * 6);

	for (int32 rowIndex = 0; rowIndex < TotalRows; rowIndex++) {
		for (int32 colIndex = 0; colIndex < TotalCols; colIndex++) {
			InTangents.Add(FVector3f(1.0f, 0.0f, 0.0f));
		}
	}
	return InTangents;
}

TArray<FVector3f> UUST_MathUtils::GetPlaneGridBiNormals(int32 TotalRows, int32 TotalCols)
{
	TArray<FVector3f> InBiNormals;
	InBiNormals.Reserve(TotalRows * TotalCols * 6);

	for (int32 rowIndex = 0; rowIndex < TotalRows; rowIndex++) {
		for (int32 colIndex = 0; colIndex < TotalCols; colIndex++) {
			InBiNormals.Add(FVector3f(0.0f, 1.0f, 0.0f));
		}
	}
	return InBiNormals;
}

TArray<FVector2f> UUST_MathUtils::GetUVsOfVertices(int32 TotalRows, int32 TotalCols, int32 SizeX, int32 SizeY)
{
	TArray<FVector2f> uvs;
	uvs.Reserve(TotalRows* TotalCols);

	for (int rowIndex = 0; rowIndex < TotalRows; ++rowIndex)
	{
		for (int colIndex = 0; colIndex < TotalCols; ++colIndex)
		{
			int i = colIndex + rowIndex  * TotalRows;

			float u = (float)colIndex / (TotalCols - 1);
			float v = (float)rowIndex / (TotalRows - 1);

			uvs[i] = FVector2f(u, v);
		}
	}

	return uvs;
}

TArray<FVector3f> UUST_MathUtils::GetPositionsOfVertices()
{
	return TArray<FVector3f>();
}

FTransform UUST_MathUtils::CalculateTransformOfNextTile(FTransform ActorTransform, float RotX, float RotY, float RotZ, FVector ForwardVector, float Magnitude)
{
	FVector NextPlaneLocation = (ForwardVector * Magnitude) + ActorTransform.GetLocation();
	FRotator NextPlaneRotation = UKismetMathLibrary::TransformRotation(ActorTransform, FRotator(RotY, RotZ, RotX));

	return FTransform(NextPlaneRotation, NextPlaneLocation, ActorTransform.GetScale3D());

}

bool UUST_MathUtils::CheckTextIsNumeric(FText Text)
{
	FString str = Text.ToString();
	if (str.Len() == 0) return false;

	const FRegexPattern pattern(TEXT("^[+-]?([0-9]*[.])?[0-9]+$"));
	FRegexMatcher matcher(pattern, str);

	if (matcher.FindNext())
	{
		return true;
	}
	return false;
}

float UUST_MathUtils::InchesToCentiMeters(float InInches)
{
	// 1 inch = 2.54 cms
	float conversionFactor = 2.54;
	return InInches * conversionFactor;
}

float UUST_MathUtils::CentiMetersToInches(float InCentimeters)
{
	float conversionFactor = 1/ 2.54;
	return InCentimeters * conversionFactor;
}


float UUST_MathUtils::Diagonal_InInches_FromCentiMeters(float Width_cms, float Height_cms)
{
	// 1 cm = 0.39 inches
	float conversionFactor = 1 / 2.54;

	float Diagonal_cms = FMath::Sqrt(Width_cms * Width_cms + Height_cms * Height_cms);
	return Diagonal_cms * conversionFactor;
}

float UUST_MathUtils::Diagonal_InPixels_FromPixels(float Horizontal_Pixels, float Vertical_Pixels) {
	float Diagonal_Pixels = FMath::Sqrt(Horizontal_Pixels * Horizontal_Pixels + Vertical_Pixels * Vertical_Pixels);
	return Diagonal_Pixels;
}

float UUST_MathUtils::CalculatePPI(float Diagonal_Pixels, float Diagonal_Inches) {
	//  PPI (Pixels Per Inch) measures pixel density. It tells us how many pixels fit into one inch of screen space.
	float PPI = Diagonal_Pixels / Diagonal_Inches;
	return PPI;
}

float UUST_MathUtils::ConvertPixelsToCentiMeters(float Pixels, float PPI) {
	float centimeters = Pixels * (2.54 / PPI);
	return centimeters;
}

float UUST_MathUtils::ConvertCentiMetersToPixels(float Centimeters, float PPI) {
	float pixels = (Centimeters * PPI) / 2.54;
	return pixels;
}

float UUST_MathUtils::MapFloatRange(float value, float inMin, float inMax, float outMin, float outMax)
{
	float ClampedValue = FMath::Clamp(value, inMin, inMax);

	// Map the value from the input range to the output range
	return FMath::Lerp(outMin, outMax, (ClampedValue - inMin) / (inMax - inMin));
}

FVector3f UUST_MathUtils::ConvertToFVector3f(FVector DoublePrecisionVector)
{
	FVector3f SinglePrecisionVector;

	// Assign each component individually
	SinglePrecisionVector.X = static_cast<float>(DoublePrecisionVector.X);
	SinglePrecisionVector.Y = static_cast<float>(DoublePrecisionVector.Y);
	SinglePrecisionVector.Z = static_cast<float>(DoublePrecisionVector.Z);

	return SinglePrecisionVector;
}

FTransform UUST_MathUtils::CalculateRelativeTransform(const FTransform& TransformA, const FTransform& TransformB)
{
	// Get the relative transform of TransformA relative to TransformB
	FTransform RelativeTransform = TransformA.GetRelativeTransform(TransformB);

	return RelativeTransform;
}

void UUST_MathUtils::GenerateMeshCoordinates(float TileWidth, float TileHeight, int32 TileNumX, int32 TileNumY,
	int32 ScreenNumX, int32 ScreenNumY, float OffsetX, float OffsetY,float TileAngle, FVector StartingPosition,AActor* PositionActor,
	TArray<FTransform>& MeshCoordinates)
{
	const float ScreenWidth = TileWidth*TileNumX;
	const float ScreenHeight = TileHeight*TileNumY;

	const float TotalScreensWidth = ScreenWidth*ScreenNumX;
	const float TotalScreensHeight = ScreenHeight*ScreenNumY;

	const float TotalWidth = TotalScreensWidth + (OffsetX+(ScreenNumX-1));
	const float TotalHeight = TotalScreensHeight + (OffsetY+(ScreenNumY-1));

	const int TotalTileX = ScreenNumX*TileNumX;
	const int TotalTileY = ScreenNumY*TileNumY;

	FTransform CurrentTransform = FTransform::Identity;
	CurrentTransform.SetLocation(StartingPosition);
	
	for(int i=0;i<TotalTileY;i++)
	{
		for(int j=0;j<TotalTileX;j++)
		{
			PositionActor->SetActorTransform(CurrentTransform);
			MeshCoordinates.Add(CurrentTransform);
			if(((j+1) % TileNumX) == 0)
			{
				CurrentTransform =  CalculateTransformOfNextTile(PositionActor->GetActorTransform(),0.f, 0.f, TileAngle, PositionActor->GetActorForwardVector(), TileWidth+OffsetX);
			}
			else
			{
				CurrentTransform = CalculateTransformOfNextTile(PositionActor->GetActorTransform(),0.f, 0.f, TileAngle, PositionActor->GetActorForwardVector(), TileWidth);
			}
		}
		if(((i+1) % TileNumY) == 0)
		{
			CurrentTransform.SetLocation( FVector(StartingPosition.X,StartingPosition.Y,CurrentTransform.GetLocation().Z+TileHeight+OffsetY));
		}
		else
		{
			CurrentTransform.SetLocation( FVector(StartingPosition.X,StartingPosition.Y,CurrentTransform.GetLocation().Z+TileHeight));
		}
		CurrentTransform.SetRotation(FQuat::MakeFromEuler(FVector(0, 0, 0)));
	}
	
}

int32 UUST_MathUtils::Get1dFrom2D(int32 X, int32 Y, int32 RowCount)
{
	return Y * RowCount + X;
}

float UUST_MathUtils::CalculateDistanceForFullFOV(float ObjectLengthOrHeight, float FOVAngle)
{
	// Convert FOV angle from degrees to radians
	float FOVRadians = FMath::DegreesToRadians(FOVAngle);

	// Calculate the distance using trigonometry
	float Distance = (ObjectLengthOrHeight / 2) / FMath::Tan(FOVRadians / 2);

	return Distance;
}

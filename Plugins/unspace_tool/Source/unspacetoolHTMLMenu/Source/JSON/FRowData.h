// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FRowData.generated.h"

/**
 * 
 */
USTRUCT(Category="JSON|RowData", BlueprintType)
struct FRowData 
{
	GENERATED_BODY()
	UPROPERTY(Category="JSON|RowData", EditAnywhere, BlueprintReadWrite)
	FString RowIndex;
	
	UPROPERTY(Category="JSON|RowData", EditAnywhere, BlueprintReadWrite)
	FString FirstDropdownValue;

	UPROPERTY(Category="JSON|RowData", EditAnywhere, BlueprintReadWrite)
	FString SecondDropdownValue;

	UPROPERTY(Category="JSON|RowData", EditAnywhere, BlueprintReadWrite)
	FString BetweenDropdownValue;

	/*UFUNCTION(BlueprintCallable, Category = "unspacetoolHTMLUtils")
	void SetRowData(FString InRowIndex, FString InFirstDropdownValue, FString InSecondDropdownValue, FString InBetweenDropdownValue)
	{
		RowIndex = InRowIndex;
		FirstDropdownValue = InFirstDropdownValue;
		SecondDropdownValue = InSecondDropdownValue;
		BetweenDropdownValue = InBetweenDropdownValue;
	}*/
};

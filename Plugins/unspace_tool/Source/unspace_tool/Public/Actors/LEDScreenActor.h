// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataClasses/UST_TileData.h"
#include "DataClasses/LEDBuildData.h"
#include "DataClasses/LEDScreenData.h"
#include "Enums/UST_Types.h"
#include "LEDScreenActor.generated.h"



UCLASS()
class UNSPACE_TOOL_API ALEDScreenActor : public AActor
{
	GENERATED_BODY()
public:
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Screen Info")
	//EScreenType ScreenType;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Screen Info")
	int32 ScreenIndex = -1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Screen Info")
	FString Prefix = TEXT("");

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Screen Info")
	FLEDScreenData LEDScreenData;

public:	
	// Sets default values for this actor's properties
	ALEDScreenActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

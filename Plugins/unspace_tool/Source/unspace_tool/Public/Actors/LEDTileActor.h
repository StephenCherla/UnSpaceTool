// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataClasses/UST_TileData.h"
#include "DataClasses/LEDBuildData.h"
#include "LEDTileActor.generated.h"

UCLASS()
class UNSPACE_TOOL_API ALEDTileActor : public AActor
{
	GENERATED_BODY()

public:
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tiles Data")
	//FUST_TileData TileInfo;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileActor Info")
	ETileActorType TileActorType = ETileActorType::None;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Screen Info")
	FString Prefix = TEXT("");

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Screen Info")
	FString ScreenName = TEXT("");

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Screen Info")
	//FLEDTileData LEDTileData;

	
public:	
	// Sets default values for this actor's properties
	ALEDTileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

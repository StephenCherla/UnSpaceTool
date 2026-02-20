// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <DataClasses/LEDBuildData.h>
#include "LEDScreenGenerator.generated.h"


UCLASS()
class UNSPACE_TOOL_API ALEDScreenGenerator : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Build Data")
	FLEDBuildData LEDBuildData;
	
public:	
	// Sets default values for this actor's properties
	ALEDScreenGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

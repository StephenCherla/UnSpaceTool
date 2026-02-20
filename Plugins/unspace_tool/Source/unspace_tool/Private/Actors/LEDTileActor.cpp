// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LEDTileActor.h"

// Sets default values
ALEDTileActor::ALEDTileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALEDTileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALEDTileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


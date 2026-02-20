// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LEDScreenActor.h"

// Sets default values
ALEDScreenActor::ALEDScreenActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALEDScreenActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALEDScreenActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


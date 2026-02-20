// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LEDScreenGenerator.h"

// Sets default values
ALEDScreenGenerator::ALEDScreenGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALEDScreenGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALEDScreenGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


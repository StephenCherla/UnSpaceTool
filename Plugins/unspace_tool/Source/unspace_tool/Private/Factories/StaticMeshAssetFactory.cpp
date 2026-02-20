// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/StaticMeshAssetFactory.h"

UStaticMeshAssetFactory::UStaticMeshAssetFactory()
{
	// Set this to true if you want to be able to create new, empty Assets from
	// the editor.
	bCreateNew = true;

	bEditAfterNew = true;

	// Tells the Editor which Asset type this UFactory can import
	SupportedClass = UStaticMesh::StaticClass();
}

UObject* UStaticMeshAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	auto StaticMeshAsset = NewObject<UStaticMesh>(InParent, InClass, InName, Flags);
	return StaticMeshAsset;
}

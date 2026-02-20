// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Factories/Factory.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "StaticMeshAssetFactory.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNSPACE_TOOL_API UStaticMeshAssetFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UStaticMeshAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UST_Auth_API.generated.h"

UCLASS()
class UNSPACE_TOOL_API USaveAuthDetails : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveDetails")
	FString AuthDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveDetails")
	FString SessionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveDetails")
	FString AccessToken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveDetails")
	FDateTime ExpiryTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveDetails")
	FString UserAuthToken;
	
};

/**
 * 
 */
UCLASS()
class UNSPACE_TOOL_API UUST_Auth_API : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category = "QSUtility | UploadToQSServer")
	static FString GetLoginAPI_URL();

	UFUNCTION(BlueprintCallable,Category = "QSUtility | UploadToQSServer")
	static FString GetVerifyOTPAPI_URL();

	UFUNCTION(BlueprintCallable,Category = "QSUtility | UploadToQSServer")
	static FString GetRefreshAuthTokenAPI_URL();
	
	static FString GetClientID();

	static FString GetClientSecret();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableText.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/Object.h"
#include "Interfaces/IHttpRequest.h"
#include "UST_AuthModule.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCodeSentSuccesfullDelegate, const bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTokenReceivedDelegate, const bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReceivedDataFromRequests);
UCLASS()
class UNSPACE_TOOL_API UUST_AuthModule : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UST_Auth_Utils")
	void InitiatePasswordlessLogin(const FString& userEmail);

	void HandlePasswordlessLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	UFUNCTION(BlueprintCallable, Category = "UST_Auth_Utils")
	void VerifyCodeAndRetrieveToken(const FString& userEmail, const FString& code);

	void HandleTokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	
	void SendPostRequest(const FString& URL, const FString& Payload, void (UUST_AuthModule::*ResponseHandler)(FHttpRequestPtr, FHttpResponsePtr, bool));

	UFUNCTION(BlueprintCallable, Category = "UST_Auth_Utils")
	static void SaveAuthData(FString AuthDetails,  FString SessionID,FString AccessToken, FDateTime ExpiryTime);

	UFUNCTION(BlueprintCallable, Category = "UST_Auth_Utils")
	static class USaveAuthDetails* LoadAuthData();

	void OnLoginSuccessful(FString UserAuthToken);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UVerticalBox* LoginFlow;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UWidgetSwitcher* MessageWidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UVerticalBox* EmailHolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UVerticalBox* OTPHolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UEditableText* Email;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UEditableText* OTP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UButton* Continue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UButton* Verify;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UButton* Register;

	UFUNCTION(BlueprintCallable, Category = "UST_Auth_Utils")
	void SetupLoginFlow();

	UFUNCTION(BlueprintCallable, Category = "UST_Auth_Utils")
	void OnClickContinue();
	UFUNCTION(BlueprintCallable, Category = "UST_Auth_Utils")
	void OnClickVerify();
	UFUNCTION(BlueprintCallable, Category = "UST_Auth_Utils")
	void OnClickRegister();
	UFUNCTION(BlueprintCallable, Category = "UST_Auth_Utils")
	void CheckForSavedLogin();
	
private:
	UPROPERTY(BlueprintAssignable)
	FCodeSentSuccesfullDelegate CodeSentSuccessfully;
	
	UPROPERTY(BlueprintAssignable)
	FTokenReceivedDelegate TokenReceived;

	UPROPERTY(BlueprintAssignable)
	FReceivedDataFromRequests ReceivedDataFromRequests;

	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Modules/UST_AuthModule.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "DataClasses/UST_Auth_API.h"
#include "Kismet/GameplayStatics.h"

void UUST_AuthModule::InitiatePasswordlessLogin(const FString& userEmail)
{
	FString URL = UUST_Auth_API::GetLoginAPI_URL();
	FString ClientID = UUST_Auth_API::GetClientID();
	FString ClientSecret = UUST_Auth_API::GetClientSecret();

	FString Payload = FString::Printf(
		TEXT("{\"client_id\":\"%s\",\"client_secret\":\"%s\",\"connection\":\"email\",\"email\":\"%s\",\"send\":\"code\",\"authParams\":{\"scope\":\"openid\"}}"),
		*ClientID, *ClientSecret, *userEmail
	);

	SendPostRequest(URL, Payload, &UUST_AuthModule::HandlePasswordlessLoginResponse);
}

void UUST_AuthModule::HandlePasswordlessLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	FString ResponseBody = Response->GetContentAsString();
	if (bSuccess && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok) {
		UE_LOG(LogTemp,Error,TEXT("Valid Code request %s"),*ResponseBody);
		//ShowOTPScreen
		UE_LOG(LogTemp,Warning,TEXT("Code sent to email!"));
		CodeSentSuccessfully.Broadcast(true);
		if(EmailHolder)
			EmailHolder->SetVisibility(ESlateVisibility::Collapsed);
		if(OTPHolder)
			OTPHolder->SetVisibility(ESlateVisibility::Visible);
		if(MessageWidgetSwitcher)
			MessageWidgetSwitcher->SetActiveWidgetIndex(0);
	} else {
		UE_LOG(LogTemp,Error,TEXT("Inavild Code request %s"),*ResponseBody);
		CodeSentSuccessfully.Broadcast(false);
		MessageWidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void UUST_AuthModule::VerifyCodeAndRetrieveToken(const FString& userEmail, const FString& code)
{
	FString URL = UUST_Auth_API::GetVerifyOTPAPI_URL();
	FString ClientID = UUST_Auth_API::GetClientID();
	FString ClientSecret = UUST_Auth_API::GetClientSecret();
	FString Payload = FString::Printf(
		TEXT("{\"grant_type\":\"http://auth0.com/oauth/grant-type/passwordless/otp\",\"client_id\":\"%s\",\"client_secret\":\"%s\",\"username\":\"%s\",\"otp\":\"%s\",\"realm\":\"email\",\"scope\":\"openid profile email\"}"),
		*ClientID, *ClientSecret, *userEmail, *code
	);
	SendPostRequest(URL, Payload, &UUST_AuthModule::HandleTokenResponse);
}

void UUST_AuthModule::HandleTokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok) {
		FString ResponseBody = Response->GetContentAsString();
		UE_LOG(LogTemp,Warning,TEXT("HandleTokenResponse %s"),*ResponseBody);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString AccessToken = JsonObject->GetStringField("access_token");
			FString IdToken = JsonObject->GetStringField("id_token");
			FString Scope = JsonObject->GetStringField("scope");
			int32 ExpiresIn = JsonObject->GetNumberField("expires_in");
			FString TokenType = JsonObject->GetStringField("token_type");
			FDateTime ExpiryTime  = FDateTime::UtcNow()+FTimespan(0, 0, ExpiresIn);
			//FDateTime::ParseIso8601(*(FString::FromInt(ExpiresIn)),ExpiryTime);
			SaveAuthData(ResponseBody,IdToken,AccessToken,ExpiryTime);
			const FString UserAuthToken = FString::Printf(TEXT("%s %s"),*IdToken,*AccessToken);
			OnLoginSuccessful(UserAuthToken);
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Inavild response %s"),*ResponseBody);
			TokenReceived.Broadcast(false);
			MessageWidgetSwitcher->SetActiveWidgetIndex(2);
		}
	} else {
		FString ResponseBody = Response->GetContentAsString();
		UE_LOG(LogTemp,Warning,TEXT("Auth Failed %s"),*ResponseBody);
		TokenReceived.Broadcast(false);
		MessageWidgetSwitcher->SetActiveWidgetIndex(2);
	}
	
}

void UUST_AuthModule::SendPostRequest(const FString& URL, const FString& Payload,
                                      void(UUST_AuthModule::* ResponseHandler)(FHttpRequestPtr, FHttpResponsePtr, bool))
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader("Content-Type", "application/json");
	HttpRequest->SetURL(URL);
	HttpRequest->SetContentAsString(Payload);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, ResponseHandler);
	HttpRequest->ProcessRequest();
}

void UUST_AuthModule::SaveAuthData(FString AuthDetails, FString SessionID, FString AccessToken, FDateTime ExpiryTime)
{
	USaveAuthDetails* AuthDetailsObj = NewObject<USaveAuthDetails>();
	AuthDetailsObj->AuthDetails = AuthDetails;
	AuthDetailsObj->SessionID = SessionID;
	AuthDetailsObj->AccessToken = AccessToken;
	AuthDetailsObj->ExpiryTime = ExpiryTime;
	AuthDetailsObj->UserAuthToken = FString::Printf(TEXT("%s %s"),*SessionID,*AccessToken);
	UGameplayStatics::SaveGameToSlot(AuthDetailsObj, "QuickStitchAuthDetails", 0);
}

USaveAuthDetails* UUST_AuthModule::LoadAuthData()
{
	USaveAuthDetails* AuthDetailsObj = Cast<USaveAuthDetails>(UGameplayStatics::LoadGameFromSlot("QuickStitchAuthDetails", 0));
	if(!AuthDetailsObj) //Check for AuthDetails, If NotAvailable Create a new slot
		{
		AuthDetailsObj = NewObject<USaveAuthDetails>();
		UGameplayStatics::SaveGameToSlot(AuthDetailsObj, "QuickStitchAuthDetails", 0);
		}
	return AuthDetailsObj;
}

void UUST_AuthModule::OnLoginSuccessful(FString UserAuthToken)
{
	//ShowMainScreen
	if(WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(2);
		TokenReceived.Broadcast(true);
	}
}

void UUST_AuthModule::SetupLoginFlow()
{
	if(EmailHolder)
		EmailHolder->SetVisibility(ESlateVisibility::Visible);
	if(OTPHolder)
		OTPHolder->SetVisibility(ESlateVisibility::Collapsed);
	if(MessageWidgetSwitcher)
		MessageWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UUST_AuthModule::OnClickContinue()
{
	InitiatePasswordlessLogin(Email->GetText().ToString());
}

void UUST_AuthModule::OnClickVerify()
{
	VerifyCodeAndRetrieveToken(Email->GetText().ToString(),OTP->GetText().ToString());
}

void UUST_AuthModule::OnClickRegister()
{
	const FString URL = "https://www.google.com";
	FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
}

void UUST_AuthModule::CheckForSavedLogin()
{
	USaveAuthDetails* AuthDetails = LoadAuthData();
	UE_LOG(LogTemp,Warning,TEXT("Check for SavedLogin Called %s"),*(AuthDetails->ExpiryTime.ToString()));
	if(!AuthDetails->AccessToken.IsEmpty())
	{
		if(FDateTime::UtcNow() < AuthDetails->ExpiryTime)
		{
			UE_LOG(LogTemp,Warning,TEXT("Token NotExpired %s"),*(FDateTime::UtcNow().ToString()));
			const FString UserAuthToken = FString::Printf(TEXT("%s %s"),*(AuthDetails->SessionID),*(AuthDetails->AccessToken));
			OnLoginSuccessful(UserAuthToken);
		}
	}
}

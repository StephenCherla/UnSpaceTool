// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\DataClasses\UST_Auth_API.h"

FString UUST_Auth_API::GetLoginAPI_URL()
{
	const FString ServerAddress =FString::Printf(TEXT("https://dev-ii6j3u5b2l63a6r8.us.auth0.com/"));
	const FString Endpoint = FString::Printf(TEXT("%spasswordless/start"), *ServerAddress);
	return Endpoint;
}

FString UUST_Auth_API::GetVerifyOTPAPI_URL()
{
	const FString ServerAddress =FString::Printf(TEXT("https://dev-ii6j3u5b2l63a6r8.us.auth0.com/"));
	const FString Endpoint = FString::Printf(TEXT("%soauth/token"), *ServerAddress);
	return Endpoint;
}

FString UUST_Auth_API::GetRefreshAuthTokenAPI_URL()
{
	const FString ServerAddress =FString::Printf(TEXT("www.google.com/"));
	const FString Endpoint = FString::Printf(TEXT("%sapi/auth/refresh"), *ServerAddress);
	return Endpoint;
}

FString UUST_Auth_API::GetClientID()
{
	return  FString::Printf(TEXT("5wo9vRYPmzrL3zdhUpd4kF9mObWZZRDe"));
}

FString UUST_Auth_API::GetClientSecret()
{
	return  FString::Printf(TEXT("if8Ino3BEi38S0hoaXa2ld5imLyW5_W1-wPyy28OIJHESF_8gwNq3oGR4rhWXOHA"));
}

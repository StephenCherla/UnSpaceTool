// Copyright 2018-2020 David Romanski (Socke). All Rights Reserved.

#include "../Public/unspacetoolHTMLMenuMainBPLibrary.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UunspacetoolHTMLMenuMainBPLibrary* UunspacetoolHTMLMenuMainBPLibrary::unspacetoolHTMLMenuMainBPLibrary;

UunspacetoolHTMLMenuMainBPLibrary::UunspacetoolHTMLMenuMainBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer){
	unspacetoolHTMLMenuMainBPLibrary = this;
}

UunspacetoolHTMLMenuMainBPLibrary* UunspacetoolHTMLMenuMainBPLibrary::getunspacetoolHTMLMenuMainBPLibrary(){
	return unspacetoolHTMLMenuMainBPLibrary;
}

void UunspacetoolHTMLMenuMainBPLibrary::initWebsocketServer(UunspacetoolHTMLMenu* menuP){
	menu = menuP;

	if (websocketServerThread == nullptr) {
		//UE_LOG(LogTemp, Warning, TEXT("unspacetoolHTMLMenu initWebserver 1"));
		websocketServerThread = new FunspacetoolHTMLMenuWebsocketServerThread(this);
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, TEXT("appReactivtedDelegate 1"));
	}
	else {
		if (isInConnectionProcess)
			return;
		isInConnectionProcess = true;
		//UE_LOG(LogTemp, Warning, TEXT("unspacetoolHTMLMenu initWebserver 2"));
		websocketServerThread->allowNewConnection();
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, TEXT("appReactivtedDelegate 2"));*/
	}

}

void UunspacetoolHTMLMenuMainBPLibrary::callJavascript(FString javascript){
	/*AsyncTask(ENamedThreads::GameThread, [javascript]() {
		UunspacetoolHTMLMenuMainBPLibrary::getunspacetoolHTMLMenuMainBPLibrary()->getMenu()->ExecuteJavascript(javascript);
	});*/

	if (menu->IsValidLowLevelFast()) {
		menu->ExecuteJavascript(javascript);
	}
	
}

void UunspacetoolHTMLMenuMainBPLibrary::handleWebsocketData(FString& data){
	menu->handleWebsocketData(data);
}

UunspacetoolHTMLMenu* UunspacetoolHTMLMenuMainBPLibrary::getMenu() {
	return menu;
}

void UunspacetoolHTMLMenuMainBPLibrary::setInConnectionProcess(bool connectionp)
{
	isInConnectionProcess = connectionp;
}

TSharedPtr<FJsonObject> UunspacetoolHTMLMenuMainBPLibrary::ParseJsonString(const FString& JsonString)
{
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			return JsonObject;
		}
		return nullptr;
	}
}

FRowData UunspacetoolHTMLMenuMainBPLibrary::GetRowDataFromString(FString JsonString)
{
	FRowData RowData;
	TSharedPtr<FJsonObject> JsonObject = ParseJsonString(JsonString);
	if (JsonObject.IsValid())
	{
		RowData.RowIndex = JsonObject->GetStringField("rowIndex");
		RowData.FirstDropdownValue = JsonObject->GetStringField("firstDropdownValue");
		RowData.SecondDropdownValue = JsonObject->GetStringField("secondDropdownValue");
		RowData.BetweenDropdownValue = JsonObject->GetStringField("betweenDropdownValue");
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON string in GetRowDataFromString"));
	}
	return RowData;
}



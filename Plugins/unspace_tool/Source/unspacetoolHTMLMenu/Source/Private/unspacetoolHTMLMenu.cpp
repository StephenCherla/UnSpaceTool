// Copyright 2018-2020 David Romanski (Socke). All Rights Reserved.

#include "../Public/unspacetoolHTMLMenu.h"
#include "SWebBrowser.h"
#include "Runtime/WebBrowser/Public/WebBrowserModule.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Async/TaskGraphInterfaces.h"
#include "UObject/ConstructorHelpers.h"

#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/HAL/FileManager.h"

#if WITH_EDITOR
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialExpressionMaterialFunctionCall.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionTextureSampleParameter2D.h"
#include "Materials/MaterialFunction.h"
#include "Factories/MaterialFactoryNew.h"
#include "PackageHelperFunctions.h"
#endif

#define LOCTEXT_NAMESPACE "unspacetoolHTMLMenuMain"

/////////////////////////////////////////////////////
// UunspacetoolHTMLMenu


TMap<FString, UJavascriptBean*> UunspacetoolHTMLMenu::javascriptBeans;

UunspacetoolHTMLMenu::UunspacetoolHTMLMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;

	onJavascriptEventMouseDown.AddDynamic(this, &UunspacetoolHTMLMenu::JavascriptEventMouseDown);
	onJavascriptEventMouseUp.AddDynamic(this, &UunspacetoolHTMLMenu::JavascriptEventMouseUp);

}




void UunspacetoolHTMLMenu::JavascriptEventMouseDown(const float X, const float Y, const int32 button) {}
void UunspacetoolHTMLMenu::JavascriptEventMouseUp(const float X, const float Y, const int32 button) {}


void UunspacetoolHTMLMenu::appReactivtedDelegate() {

	UunspacetoolHTMLMenuMainBPLibrary::getunspacetoolHTMLMenuMainBPLibrary()->initWebsocketServer(this);
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, TEXT("appReactivtedDelegate 1"));*/


}

void UunspacetoolHTMLMenu::appEnterBackgroundDelegate() {
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, TEXT("appEnterBackgroundDelegate"));
	//if (websocketServerThread != nullptr) {
	//	websocketServerThread->stopThread();
	//	websocketServerThread = nullptr;
	//}
}



void UunspacetoolHTMLMenu::reconnectMobile() {
	
}

void UunspacetoolHTMLMenu::setWebsocketPort(int32 port) {
	lastWebsocketPort = port;
}

int32 UunspacetoolHTMLMenu::getWebsocketPort() {
	return lastWebsocketPort;
}





void UunspacetoolHTMLMenu::registerJavascriptEvent(const FString nameOfJavascriptFunction, UJavascriptBean*& eventTarget) {
	if (!unspacetoolHTMLMenu.IsValid())
		return;

	if (widgetID.IsEmpty())
		widgetID = FGuid::NewGuid().ToString();

	FString lowerName = nameOfJavascriptFunction.ToLower() + widgetID;

	UE_LOG(LogTemp, Warning, TEXT("unspacetoolHTMLMenu registerJavascriptEvent: %s"), *lowerName);

	UPROPERTY()
		UJavascriptBean* javascriptBean = NewObject<UJavascriptBean>(UJavascriptBean::StaticClass());
	javascriptBean->AddToRoot();
	javascriptBean->setBrowser(this);
	javascriptBeans.Add(lowerName, javascriptBean);
	eventTarget = javascriptBean;

}


void UunspacetoolHTMLMenu::triggerUE4EventWithTag(FString javascriptFunctionsName, FString ID, FString className, FString value, FString arguments) {

	FString lowerName = javascriptFunctionsName.ToLower() + widgetID;
	if (javascriptBeans.Num() == 0 || javascriptBeans.Find(lowerName) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("unspacetoolHTMLMenu Javascript Function %s is not registered."), *lowerName);
		return;
	}
	UJavascriptBean* javascriptBean = *javascriptBeans.Find(lowerName);

	//UE_LOG(LogTemp, Display, TEXT("JAVASCRIPT:%s"), *arguments);
	TArray<FString> args;
	arguments.ParseIntoArray(args, TEXT(".;_|"), true);
	AsyncTask(ENamedThreads::GameThread, [javascriptBean, ID, className, value, args]() {
		javascriptBean->onJavascriptEventTag.Broadcast(ID, className, value, args);
	});
}

void UunspacetoolHTMLMenu::triggerUE4EventWithBlank(FString javascriptFunctionsName, FString arguments) {

	FString lowerName = javascriptFunctionsName.ToLower() + widgetID;

	//UE_LOG(LogTemp, Warning, TEXT("unspacetoolHTMLMenu triggerUE4EventWithBlank: %s"), *lowerName);

	if (javascriptBeans.Num() == 0 || javascriptBeans.Find(lowerName) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("unspacetoolHTMLMenu Javascript Function %s is not registered."), *lowerName);
		return;
	}
	UJavascriptBean* javascriptBean = *javascriptBeans.Find(lowerName);

	//UE_LOG(LogTemp, Display, TEXT("JAVASCRIPT:%s"), *arguments);
	TArray<FString> args;
	arguments.ParseIntoArray(args, TEXT(".;_|"), true);
	AsyncTask(ENamedThreads::GameThread, [javascriptBean, args]() {
		javascriptBean->onJavascriptEventBlank.Broadcast(args);
		});
}

void UunspacetoolHTMLMenu::triggerUE4EventWithMap(FString javascriptFunctionsName, FString arguments) {

	FString lowerName = javascriptFunctionsName.ToLower() + widgetID;
	if (javascriptBeans.Num() == 0 || javascriptBeans.Find(lowerName) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("unspacetoolHTMLMenu Javascript Function %s is not registered."), *lowerName);
		return;
	}
	UJavascriptBean* javascriptBean = *javascriptBeans.Find(lowerName);

	FStringTMap stringTMap;
	//UE_LOG(LogTemp, Display, TEXT("JAVASCRIPT:%s"), *arguments);
	TArray<FString> args;
	arguments.ParseIntoArray(args, TEXT(".;_|"), true);
	for (int i = 0; i < args.Num(); i++) {
		FString left;
		FString right;
		args[i].Split(":", &left, &right);
		stringTMap.stringMap.Add(left, right);
	}
	AsyncTask(ENamedThreads::GameThread, [javascriptBean, stringTMap]() {
		javascriptBean->onJavascriptEventMap.Broadcast(stringTMap);
		});
}

void UunspacetoolHTMLMenu::triggerUE4EventWithJSON(FString javascriptFunctionsName, FString json) {

	FString lowerName = javascriptFunctionsName.ToLower() + widgetID;
	if (javascriptBeans.Num() == 0 || javascriptBeans.Find(lowerName) == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("unspacetoolHTMLMenu Javascript Function %s is not registered."), *lowerName);
		return;
	}
	UJavascriptBean* javascriptBean = *javascriptBeans.Find(lowerName);


	AsyncTask(ENamedThreads::GameThread, [javascriptBean, json]() {
		javascriptBean->onJavascriptEventJSON.Broadcast(json);
		});
}


void UunspacetoolHTMLMenu::triggerUE4EventMouseDown(float x, float y, int32 button) {

	UunspacetoolHTMLMenu* unspacetoolHTMLMenuLambda = this;

	AsyncTask(ENamedThreads::GameThread, [unspacetoolHTMLMenuLambda, x, y, button]() {
		unspacetoolHTMLMenuLambda->onJavascriptEventMouseDown.Broadcast(x, y, button);
		});
}

void UunspacetoolHTMLMenu::triggerUE4EventMouseUp(float x, float y, int32 button) {

	UunspacetoolHTMLMenu* unspacetoolHTMLMenuLambda = this;

	AsyncTask(ENamedThreads::GameThread, [unspacetoolHTMLMenuLambda, x, y, button]() {
		unspacetoolHTMLMenuLambda->onJavascriptEventMouseUp.Broadcast(x, y, button);
		});
}

void UunspacetoolHTMLMenu::javascriptConsoleCommand(FString command) {
	AsyncTask(ENamedThreads::GameThread, [command]() {
		if (GEngine && GEngine != nullptr) {
			APlayerController* PController = UGameplayStatics::GetPlayerController(GEngine->GameViewport->GetWorld(), 0);
			if (PController) {
				PController->ConsoleCommand(command);
			}
		}
		});
}

void UunspacetoolHTMLMenu::javascriptQuitGame() {
	UunspacetoolHTMLMenu* self = this;
	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (GEngine && GEngine != nullptr) {
			FPlatformMisc::RequestExit(true);
		}
		});
}

void UunspacetoolHTMLMenu::javascriptApplyAndSaveGraphics(FString resolutionString, int32 windowMode, int32 antiAliasing, int32 shadows, float resolutionScale) {
	AsyncTask(ENamedThreads::GameThread, [resolutionString, windowMode, antiAliasing, shadows, resolutionScale]() {
		if (GEngine && GEngine != nullptr) {
			UGameUserSettings* gameSettings = GEngine->GameUserSettings;


			//RESOLUTION
			FIntPoint resolution;
			FString resX;
			FString resY;
			resolutionString.Split("x", &resX, &resY);
			resolution.X = FCString::Atoi(*resX);
			resolution.Y = FCString::Atoi(*resY);

			gameSettings->SetScreenResolution(resolution);

			//Full or Window Mode
			switch (windowMode) {
			case 0:
				gameSettings->SetFullscreenMode(EWindowMode::Type::Fullscreen);
				break;
			case 1:
				gameSettings->SetFullscreenMode(EWindowMode::Type::WindowedFullscreen);
				break;
			case 2:
				gameSettings->SetFullscreenMode(EWindowMode::Type::Windowed);
				break;
			}

			//Anti-Aliasing
			gameSettings->SetAntiAliasingQuality(antiAliasing);
			//Shadows
			gameSettings->SetShadowQuality(shadows);

			//Resolution Scale
			float rs = resolutionScale / 10;
			gameSettings->SetResolutionScaleNormalized(rs);

			gameSettings->ApplySettings(false);
			gameSettings->SaveSettings();
		}
		});
}

void UunspacetoolHTMLMenu::javascriptGetGraphics() {
	if (!unspacetoolHTMLMenu.IsValid() || unspacetoolHTMLMenu.Get() == nullptr) {
		return;
	}

	if (GEngine && GEngine != nullptr && GEngine != NULL) {
		UGameUserSettings* gameSettings = GEngine->GameUserSettings;
		FString res = FString::FromInt(gameSettings->GetScreenResolution().X) + "x" + FString::FromInt(gameSettings->GetScreenResolution().Y);
		FString windowMode = FString::FromInt(gameSettings->GetFullscreenMode());
		FString antiAliasing = FString::FromInt(gameSettings->GetAntiAliasingQuality());
		FString shadows = FString::FromInt(gameSettings->GetShadowQuality());

		float CurrentScaleNormalized;
		float CurrentScaleValue;
		float MinScaleValue;
		float MaxScaleValue;

		gameSettings->GetResolutionScaleInformationEx(CurrentScaleNormalized, CurrentScaleValue, MinScaleValue, MaxScaleValue);

		FString resolutionScale = FString::FromInt((CurrentScaleNormalized * 10));
		if (unspacetoolHTMLMenu.IsValid() && unspacetoolHTMLMenu.Get() != nullptr)
			unspacetoolHTMLMenu->ExecuteJavascript("UE4SetGraphics(\"" + res + "\"," + windowMode + "," + antiAliasing + "," + shadows + "," + resolutionScale + ",false);");
	}
}

void UunspacetoolHTMLMenu::javascriptClientTravel(FString mapOrAdress) {
	AsyncTask(ENamedThreads::GameThread, [mapOrAdress]() {
		if (GEngine && GEngine != nullptr) {
			APlayerController* PController = UGameplayStatics::GetPlayerController(GEngine->GameViewport->GetWorld(), 0);
			if (PController) {
				PController->ClientTravel(mapOrAdress, ETravelType::TRAVEL_Absolute);
			}
		}
		});
}

void UunspacetoolHTMLMenu::javascriptMousePosition(float x, float y) {
	htmlMouseCursorPositionX = x;
	htmlMouseCursorPositionY = y;
}

void UunspacetoolHTMLMenu::getHTMLMouseCursorPosition(float& x, float& y) {
	x = htmlMouseCursorPositionX;
	y = htmlMouseCursorPositionY;
}

UunspacetoolHTMLMenu* UunspacetoolHTMLMenu::generateHTML_File(FString sourceHTML_File, FString targetHTML_Dir, bool overwriteExistingFile, bool onlyInEditor) {
	//run only in editor
	if (onlyInEditor) {
#if !WITH_EDITOR
		return this;
#endif
}


	if (!FPaths::FileExists(sourceHTML_File)) {
		UE_LOG(LogTemp, Error, TEXT("(56)HTML Menu: HTML File not found: %s"), *sourceHTML_File);
		return this;
	}

	//dir not file 
	if (FPaths::FileExists(targetHTML_Dir) || targetHTML_Dir.Contains(".html")) {
		targetHTML_Dir = FPaths::GetPath(targetHTML_Dir);
	}

	FString sourceHTML_FileName = FPaths::GetCleanFilename(sourceHTML_File);

	FString unspacetoolHTMLMenuDir = FPaths::ProjectDir() + targetHTML_Dir;
	//unspacetoolHTMLMenuDir = FPaths::ConvertRelativePathToFull(unspacetoolHTMLMenuDir);

	//exist? then just load it. //todo: check file time stamp
	if (!overwriteExistingFile && FPaths::FileExists(unspacetoolHTMLMenuDir + "/" + sourceHTML_FileName)) {
		return this;
	}

	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!platformFile.CreateDirectoryTree(*(FPaths::GetPath(unspacetoolHTMLMenuDir)))) {
		UE_LOG(LogTemp, Error, TEXT("(47)HTML Menu: Can't create Directory: %s"), *unspacetoolHTMLMenuDir);
		return this;
	}


	createAndMinifyunspacetoolHTMLMenu(sourceHTML_File, unspacetoolHTMLMenuDir);

	return this;

}

void UunspacetoolHTMLMenu::createAndMinifyunspacetoolHTMLMenu(FString sourceHTML_File, FString targetDir) {
	FString sourceDir = FPaths::GetPath(sourceHTML_File);


	FString content;
	bool isFileRead = FFileHelper::LoadFileToString(content, *sourceHTML_File);
	content = content.Replace(TEXT("\r"), TEXT(""));
	content = content.Replace(TEXT("\t"), TEXT(""));
	TArray<FString> lines;
	int32 lineCount = content.ParseIntoArray(lines, TEXT("\n"), true);
	content.Empty();

	for (int32 i = 0; i < lineCount; i++) {
		FString s = lines[i];
		//javascript includes
		if (s.Contains("<script") && s.Contains("</script>") && s.Contains(".js\"")) {
			s = s.Replace(TEXT("type=\"text/javascript\""), TEXT(""));
			FString left;
			FString right;
			s.Split(".js", &left, &right);
			FString js = left;
			js.Split("src=\"", &left, &right);
			FString url = sourceDir + "/" + right + ".js";

			if (!FPaths::FileExists(url)) {
				UE_LOG(LogTemp, Warning, TEXT("Can't read js file: %s"), *url);
				content.Append(s);
				continue;
			}

			FString jsContent;
			if (FFileHelper::LoadFileToString(jsContent, *url)) {
				jsContent = removeJavascriptComments(jsContent);//jsContent.Replace(TEXT("\n"), TEXT(""));
																//UE_LOG(LogTemp, Display, TEXT("%s"), *cssContent);
				content.Append("<script>" + jsContent + "</script>");
			}
		}
		else {
			//css includes
			if (s.Contains("<link") && (s.Contains(".css\"") || s.Contains(".css'")) && (s.Contains("href=\"") || s.Contains("href=\'"))) {
				s = s.Replace(TEXT("'"), TEXT("\""));
				FString left;
				FString right;
				s.Split(".css", &left, &right);
				s = left;
				s.Split("href=\"", &left, &right);
				FString url = sourceDir + "/" + right + ".css";
				if (!FPaths::FileExists(url)) {
					UE_LOG(LogTemp, Warning, TEXT("Can't read css file: %s"), *url);
					continue;
				}

				FString cssDir = FPaths::GetPath(url);
				FString cssContent;
				if (FFileHelper::LoadFileToString(cssContent, *url)) {
					cssContent = cssContent.Replace(TEXT("\n"), TEXT(""));
					cssContent = replaceCSSImages(cssContent, cssDir);
					cssContent = replaceCSSFonts(cssContent, cssDir);
					//UE_LOG(LogTemp, Display, TEXT("%s"), *cssContent);
					content.Append("<style>" + cssContent + "</style>");
				}
			}
			else {
				//without comments
				if (s.Contains("//")) {
					FString b = s.TrimStart();
					if (!b.StartsWith("//")) {
						content.Append(s);
					}
				}
				else {
					if (s.Contains("<img")) {
						s = replaceHTMLImages(s, sourceDir);
					}
					content.Append(s);
				}
			}
		}
	}

	FFileHelper::SaveStringToFile(content, *(targetDir + "/" + FPaths::GetCleanFilename(sourceHTML_File)));

}



FString UunspacetoolHTMLMenu::replaceCSSImages(FString content, FString cssURL) {
	FString out;

	content = content.Replace(TEXT("url(\""), TEXT("url(\"\n"));
	content = content.Replace(TEXT(".png\")"), TEXT(".png\n\")"));
	content = content.Replace(TEXT(".gif\")"), TEXT(".gif\n\")"));
	content = content.Replace(TEXT(".jpg\")"), TEXT(".jpg\n\")"));
	content = content.Replace(TEXT(".jpeg\")"), TEXT(".jpeg\n\")"));

	content = content.Replace(TEXT("url('"), TEXT("url('\n"));
	content = content.Replace(TEXT(".png')"), TEXT(".png\n')"));
	content = content.Replace(TEXT(".gif')"), TEXT(".gif\n')"));
	content = content.Replace(TEXT(".jpg')"), TEXT(".jpg\n')"));
	content = content.Replace(TEXT(".jpeg')"), TEXT(".jpeg\n')"));

	content = content.Replace(TEXT("url("), TEXT("url(\n"));
	content = content.Replace(TEXT(".png)"), TEXT(".png\n)"));
	content = content.Replace(TEXT(".gif)"), TEXT(".gif\n)"));
	content = content.Replace(TEXT(".jpg)"), TEXT(".jpg\n)"));
	content = content.Replace(TEXT(".jpeg)"), TEXT(".jpeg\n)"));

	TArray<FString> lines;
	int32 lineCount = content.ParseIntoArray(lines, TEXT("\n"), true);
	for (int32 i = 0; i < lineCount; i++) {
		FString s = lines[i];
		if (s.Contains(".png") || s.Contains(".gif") || s.Contains(".jpg") || s.Contains(".jpeg")) {
			FString image = cssURL + "/" + s;
			if (FPaths::FileExists(image)) {
				TArray<uint8> fileData;
				if (FFileHelper::LoadFileToArray(fileData, *image)) {
					FString base64Image = FBase64::Encode(fileData);

					out += "data:image/" + FPaths::GetExtension(*image, true) + ";base64," + base64Image;
					continue;
				}
			}
		}

		out += s;
	}
	return out;
}

FString UunspacetoolHTMLMenu::replaceCSSFonts(FString content, FString cssURL) {
	FString out;

	content = content.Replace(TEXT("url(\""), TEXT("url(\"\n"));
	content = content.Replace(TEXT(".woff2\")"), TEXT(".woff2\n\")"));
	content = content.Replace(TEXT(".woff\")"), TEXT(".woff\n\")"));
	content = content.Replace(TEXT(".ttf\")"), TEXT(".ttf\n\")"));
	content = content.Replace(TEXT(".svg\")"), TEXT(".svg\n\")"));

	content = content.Replace(TEXT("url('"), TEXT("url('\n"));
	content = content.Replace(TEXT(".woff2')"), TEXT(".woff2\n')"));
	content = content.Replace(TEXT(".woff')"), TEXT(".woff\n')"));
	content = content.Replace(TEXT(".ttf')"), TEXT(".ttf\n')"));
	content = content.Replace(TEXT(".svg')"), TEXT(".svg\n')"));

	content = content.Replace(TEXT("url("), TEXT("url(\n"));
	content = content.Replace(TEXT(".woff2)"), TEXT(".woff2\n)"));
	content = content.Replace(TEXT(".woff)"), TEXT(".woff\n)"));
	content = content.Replace(TEXT(".ttf)"), TEXT(".ttf\n)"));
	content = content.Replace(TEXT(".svg)"), TEXT(".svg\n)"));

	TArray<FString> lines;
	int32 lineCount = content.ParseIntoArray(lines, TEXT("\n"), true);
	for (int32 i = 0; i < lineCount; i++) {
		FString s = lines[i];
		if (s.Contains(".woff2") || s.Contains(".woff") || s.Contains(".ttf") || s.Contains(".svg")) {
			FString font = cssURL + "/" + s;
			if (FPaths::FileExists(font)) {
				TArray<uint8> fileData;
				if (FFileHelper::LoadFileToArray(fileData, *font)) {
					FString base64Image = FBase64::Encode(fileData);

					out += "data:application/x-font-" + FPaths::GetExtension(*font, true) + ";charset=utf-8;base64," + base64Image;
					continue;
				}
			}
		}

		out += s;
	}
	return out;
}

FString UunspacetoolHTMLMenu::replaceHTMLImages(FString content, FString htmlURL) {
	FString out;

	content = content.Replace(TEXT("src=\""), TEXT("src=\"\n"));
	content = content.Replace(TEXT(".png\""), TEXT(".png\n\""));
	content = content.Replace(TEXT(".gif\""), TEXT(".gif\n\""));
	content = content.Replace(TEXT(".jpg\""), TEXT(".jpg\n\""));
	content = content.Replace(TEXT(".jpeg\""), TEXT(".jpeg\n\""));

	content = content.Replace(TEXT("src='"), TEXT("src='\n"));
	content = content.Replace(TEXT(".png'"), TEXT(".png\n'"));
	content = content.Replace(TEXT(".gif'"), TEXT(".gif\n'"));
	content = content.Replace(TEXT(".jpg'"), TEXT(".jpg\n'"));
	content = content.Replace(TEXT(".jpeg'"), TEXT(".jpeg\n'"));

	TArray<FString> lines;
	int32 lineCount = content.ParseIntoArray(lines, TEXT("\n"), true);
	for (int32 i = 0; i < lineCount; i++) {
		FString s = lines[i];
		if (s.Contains(".png") || s.Contains(".gif") || s.Contains(".jpg") || s.Contains(".jpeg")) {
			FString image = htmlURL + "/" + s;
			if (FPaths::FileExists(image)) {
				TArray<uint8> fileData;
				if (FFileHelper::LoadFileToArray(fileData, *image)) {
					FString base64Image = FBase64::Encode(fileData);

					out += "data:image/" + FPaths::GetExtension(*image, true) + ";base64," + base64Image;
					continue;
				}
			}
		}

		out += s;
	}
	return out;
}

FString UunspacetoolHTMLMenu::removeJavascriptComments(FString content) {
	FString out;
	TArray<FString> lines;
	int32 lineCount = content.ParseIntoArray(lines, TEXT("\n"), true);
	for (int32 i = 0; i < lineCount; i++) {
		if (lines[i].Contains("//")) {
			FString b = lines[i].TrimStart();
			if (!b.StartsWith("//")) {
				out.Append(lines[i]);
			}
		}
		else {
			out.Append(lines[i]);
		}
	}
	return out;
}

FString UunspacetoolHTMLMenu::joinFloatArray(TArray<float> data, FString Separator){
	FString Result;
	bool    First = true;
	for (int32 i = 0; i < data.Num(); i++)	{
		if (First)
		{
			First = false;
		}
		else
		{
			Result += Separator;
		}

		Result += FString::SanitizeFloat(data[i]);
	}

	return Result;
}

FString UunspacetoolHTMLMenu::fColorToFString(FColor color) {
	return FString::FromInt(color.R) + "," + FString::FromInt(color.G) + "," + FString::FromInt(color.B) + "," + FString::FromInt(color.A);
}

FString UunspacetoolHTMLMenu::ueStringArrayToJSArray(TArray<FString> ueArray){
	FString javascriptArray = "[]";
	if (ueArray.Num() > 1) {
		javascriptArray = "['" + FString::Join(ueArray, TEXT("','")) + "']";
	}
	else {
		if (ueArray.Num() == 1) {
			javascriptArray = "['" + ueArray[0] + "']";
		}
	}
	return javascriptArray;
}

FString UunspacetoolHTMLMenu::ueFloatArrayToJSArray(TArray<float> ueArray) {
	FString javascriptArray = "[]";
	if (ueArray.Num() > 1) {
		javascriptArray = "['" + joinFloatArray(ueArray, TEXT("','")) + "']";
	}
	else {
		if (ueArray.Num() == 1) {
			javascriptArray = "['" + FString::SanitizeFloat(ueArray[0]) + "']";
		}
	}
	return javascriptArray;
}

FString UunspacetoolHTMLMenu::ueFunspacetoolHTMLMenuChart2dDataArrayToJSArray(TArray<FunspacetoolHTMLMenuChart2dData> ueArray) {
	FString javascriptArray = "[";
	for (int32 i = 0; i < ueArray.Num(); i++){
		javascriptArray += "["+ FString::SanitizeFloat(ueArray[i].x) + "," + FString::SanitizeFloat(ueArray[i].x) + "]";
		if (ueArray.Num() > (i + 1)) {
			javascriptArray += ",";
		}
	}
	javascriptArray += "]";

	return javascriptArray;
}

FString UunspacetoolHTMLMenu::ueStringFloatMapToJSObjectArray(TMap<FString, float> ueMap) {

	FString javascriptObjectArray = "[";
	int32 i = 0;
	for (auto& element : ueMap) {
		javascriptObjectArray += "{ value: "+ FString::SanitizeFloat(element.Value) +", name: '"+element.Key+"' }";
		if (i < ueMap.Num()) {
			javascriptObjectArray += ",";
		}
		i++;
	}
	javascriptObjectArray += "]";
	return javascriptObjectArray;
}

UunspacetoolHTMLMenu* UunspacetoolHTMLMenu::loadHTML_File(FString htmlFile, bool removeLineBreaks) {

	if (!unspacetoolHTMLMenu.IsValid())
		return this;


	FString unspacetoolHTMLMenuDir = FPaths::ProjectDir() + htmlFile;
	//clean path
	unspacetoolHTMLMenuDir = unspacetoolHTMLMenuDir.Replace(TEXT("\\\\"), TEXT("\\"));
	unspacetoolHTMLMenuDir = unspacetoolHTMLMenuDir.Replace(TEXT("\\"), TEXT("/"));

	if (!FPaths::FileExists(unspacetoolHTMLMenuDir)) {
		UE_LOG(LogTemp, Error, TEXT("(86)File not found:%s"), *unspacetoolHTMLMenuDir);
		return this;
	}

	FString content;
	bool isFileRead = FFileHelper::LoadFileToString(content, *unspacetoolHTMLMenuDir);
	if (isFileRead) {
		if (removeLineBreaks) {
			content = content.Replace(TEXT("\t"), TEXT(""));
			content = content.Replace(TEXT("\r"), TEXT(""));
			content = content.Replace(TEXT("\n"), TEXT(""));
		}
		LoadString(content, unspacetoolHTMLMenuDir);
		unspacetoolHTMLMenu->BindUObject("uecom", this, true);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("(96)Can't read file:%s"), *unspacetoolHTMLMenuDir);
	}

	return this;
}



void UunspacetoolHTMLMenu::triggerKeyAction(EHTML_JavascriptFunction function) {
	if (!unspacetoolHTMLMenu.IsValid()) {
		return;
	}
	switch (function) {
	case EHTML_JavascriptFunction::E_Up:
		unspacetoolHTMLMenu->ExecuteJavascript("keyUp();");
		break;
	case EHTML_JavascriptFunction::E_Down:
		unspacetoolHTMLMenu->ExecuteJavascript("keyDown();");
		break;
	case EHTML_JavascriptFunction::E_Left:
		unspacetoolHTMLMenu->ExecuteJavascript("keyLeft();");
		break;
	case EHTML_JavascriptFunction::E_Right:
		unspacetoolHTMLMenu->ExecuteJavascript("keyRight();");
		break;
	case EHTML_JavascriptFunction::E_Next:
		unspacetoolHTMLMenu->ExecuteJavascript("keyNext();");
		break;
	case EHTML_JavascriptFunction::E_Previous:
		unspacetoolHTMLMenu->ExecuteJavascript("keyPrev();");
		break;
	case EHTML_JavascriptFunction::E_Choose:
		unspacetoolHTMLMenu->ExecuteJavascript("keyEventChoose();");
		break;
	}
}

void UunspacetoolHTMLMenu::LoadAllFilesFromDirectory(EHTML_DirectoryType directoryType, EHTML_FileType fileType, FString dirPath, int32& count, TArray<FString>& files, TArray<FString>& filePaths){

	FString dir;
	if (directoryType == EHTML_DirectoryType::E_ad) {
		if (dirPath.Len() < 1) {
			return;
		}
		dir = FPaths::ConvertRelativePathToFull(dirPath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + dirPath);
	}

	FPaths::NormalizeDirectoryName(dirPath);


	if (!FPaths::DirectoryExists(dir)) {
		return;
	}
	IFileManager& FileManager = IFileManager::Get();


	switch (fileType)
	{
	case EHTML_FileType::E_pdf:
		dir += "/*.pdf";
		break;
	}

	FileManager.FindFiles(files, *dir, true, false);

	if (dirPath.Len() > 0)
		dirPath += "/";
	for (int32 i = 0; i < files.Num(); i++) {
		filePaths.Add((dirPath + files[i]));
		//UE_LOG(LogTemp, Display, TEXT("->%s"), *files[i]);
	}
	count = files.Num();
}

bool UunspacetoolHTMLMenu::LoadPDF_FileIntoViewer(EHTML_DirectoryType directoryType, FString dirPath){
	if (dirPath.Len() < 1) {
		return false;
	}

	FString dir;
	if (directoryType == EHTML_DirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(dirPath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + dirPath);
	}

	if (!FPaths::FileExists(dir)) {
		return false;
	}

	TArray<uint8> fileData;
	if (!FFileHelper::LoadFileToArray(fileData, *dir)) {
		return false;
	}
	FString base64String = FBase64::Encode(fileData);
	fileData.Empty();

	ExecuteJavascript("loadPDF('"+base64String+"');");
	base64String.Empty();

	return true;
}

bool UunspacetoolHTMLMenu::loadChartLib(FString chartLib){

	if (chartLib.Len() < 1) {
		return false;
	}
	FString ProjectDir = FPaths::ProjectDir();
	FString dir = FPaths::ConvertRelativePathToFull(ProjectDir + chartLib);


	if (!FPaths::FileExists(dir)) {
		return false;
	}

	loadHTML_File(chartLib);
	return true;
}

void UunspacetoolHTMLMenu::createChart(EunspacetoolHTMLMenuChartType type, 
	FunspacetoolHTMLMenuChartTitle title,
	TArray<FString> legendData,
	EunspacetoolHTMLMenuLegendOrient legendOrient,
	EunspacetoolHTMLMenuChartPosition legendPosition,
	EunspacetoolHTMLMenuChartTooltip tooltip,
	TArray<FString> xAxisData, TArray<FunspacetoolHTMLMenuChartSeriesData> datasets, bool swapAxis,
	bool toolboxMark,
	bool toolboxDataview,
	bool toolboxMagictype,
	bool toolboxRestore,
	bool toolboxSave){

	FString titleJS = "{text: '" + title.title + "', subtext : '" + title.subtitle + "', left :";
	switch (title.position) {
	case EunspacetoolHTMLMenuChartPosition::E_left:
		titleJS += "'left'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_center:
		titleJS += "'center'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_right:
		titleJS += "'right'";
		break;
	}
	titleJS +="}";

	FString legendDataJS = ueStringArrayToJSArray(legendData);
	FString legendOrientJS = "";
	FString legendPositionJS = "";

	switch (legendOrient) {
		case EunspacetoolHTMLMenuLegendOrient::E_vertical:
			legendOrientJS = "'vertical'";
		break;
		case EunspacetoolHTMLMenuLegendOrient::E_horizontal:
			legendOrientJS = "'horizontal'";
			break;
	}

	switch (legendPosition) {
	case EunspacetoolHTMLMenuChartPosition::E_left:
		legendPositionJS = "'left'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_center:
		legendPositionJS = "'center'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_right:
		legendPositionJS = "'right'";
		break;
	}


	FString toolTipJS = "'none'";

	switch (tooltip)
	{
	case EunspacetoolHTMLMenuChartTooltip::E_none:
		toolTipJS = "'none'";
		break;
	case EunspacetoolHTMLMenuChartTooltip::E_item:
		toolTipJS = "'item'";
		break;
	case EunspacetoolHTMLMenuChartTooltip::E_axis:
		toolTipJS = "'axis'";
		break;
	}

	FString xAxisDataJS = ueStringArrayToJSArray(xAxisData);

	FString datasetsJS = "[";

	FString typeJS = "line";
	if (type == EunspacetoolHTMLMenuChartType::E_bar) {
		typeJS = "bar";
	}



	FString legendColor = "[";
	for (int32 i = 0; i < datasets.Num(); i++) {

		FString lineColor = "'rgba(" + fColorToFString(datasets[i].lineColor) + ")'";

		datasetsJS += "{ name: '" + datasets[i].name + "', type : '"+typeJS+"',lineStyle: " +
			"{color: "+ lineColor +"},";
		if (datasets[i].fillBackground) {
			datasetsJS += "areaStyle: {color: 'rgba(" + fColorToFString(datasets[i].backgroundColor) + ")'}, emphasis : {	focus: 'series'}, ";
		}
		switch (datasets[i].textInChart) {
			case EunspacetoolHTMLMenuChartLabelInChart::E_none:
				datasetsJS += "label:false,";
				break;
			case EunspacetoolHTMLMenuChartLabelInChart::E_valAndName:
				datasetsJS += "label:labelOptionValueAndName,";
				break;
			case EunspacetoolHTMLMenuChartLabelInChart::E_val:
				datasetsJS += "label:labelOptionValue,";
				break;
			case EunspacetoolHTMLMenuChartLabelInChart::E_name:
				datasetsJS += "label:labelOptionName,";
				break;
		}
	
		datasetsJS += "data:" + ueFloatArrayToJSArray(datasets[i].data) + " }";

		if ((i + 1) < datasets.Num()) {
			datasetsJS += ",";
			legendColor += lineColor + ",";
		}
		else {
			legendColor += lineColor;
		}

		
	}
	datasetsJS += "]";
	legendColor += "]";

	FString swapAxisJS = "false";
	if (swapAxis) {
		swapAxisJS = "true";
	}


	FString toolboxMarkJS = "false";
	if (toolboxMark) {
		toolboxMarkJS = "true";
	}
	FString toolboxDataviewJS = "false";
	if (toolboxDataview) {
		toolboxDataviewJS = "true";
	}
	FString toolboxMagictypeJS = "false";
	if (toolboxMagictype) {
		toolboxMagictypeJS = "true";
	}
	FString toolboxRestoreJS = "false";
	if (toolboxRestore) {
		toolboxRestoreJS = "true";
	}
	FString toolboxSaveJS = "false";
	if (toolboxSave) {
		toolboxSaveJS = "true";
	}

	FString jsCommand = "createChart(" + titleJS + "," + legendColor + 
		"," + legendOrientJS +
		"," + legendPositionJS +
		"," + legendDataJS +
		", " + toolTipJS + "," +
		xAxisDataJS + "," + datasetsJS + "," + swapAxisJS +
		"," + toolboxMarkJS +
		"," + toolboxDataviewJS +
		"," + toolboxMagictypeJS +
		"," + toolboxRestoreJS +
		"," + toolboxSaveJS + "); ";

	UE_LOG(LogTemp, Warning, TEXT("%s"), *jsCommand);

	ExecuteJavascript(jsCommand);
}


void UunspacetoolHTMLMenu::createPieChart(FunspacetoolHTMLMenuChartTitle title,
	EunspacetoolHTMLMenuLegendOrient legendOrient,
	EunspacetoolHTMLMenuChartPosition legendPosition,
	FunspacetoolHTMLMenuPieChartSeriesData dataset,
	EunspacetoolHTMLMenuChartLabelInChartWithPercent textInChart,
	bool tooltip,
	bool toolboxMark,
	bool toolboxDataview,
	bool toolboxRestore,
	bool toolboxSave){

	FString titleJS = "{text: '" + title.title + "', subtext : '" + title.subtitle + "', left :";
	switch (title.position) {
	case EunspacetoolHTMLMenuChartPosition::E_left:
		titleJS += "'left'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_center:
		titleJS += "'center'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_right:
		titleJS += "'right'";
		break;
	}
	titleJS += "}";

	FString legendOrientJS = "";
	FString legendPositionJS = "";

	switch (legendOrient) {
	case EunspacetoolHTMLMenuLegendOrient::E_vertical:
		legendOrientJS = "'vertical'";
		break;
	case EunspacetoolHTMLMenuLegendOrient::E_horizontal:
		legendOrientJS = "'horizontal'";
		break;
	}

	switch (legendPosition) {
	case EunspacetoolHTMLMenuChartPosition::E_left:
		legendPositionJS = "'left'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_center:
		legendPositionJS = "'center'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_right:
		legendPositionJS = "'right'";
		break;
	}


	FString toolTipJS = "'none'";
	if (tooltip) {
		toolTipJS = "'item'";
	}


	FString legendColor = "defaultCartColors";
	if (dataset.data.Num() <= dataset.colors.Num()) {
		legendColor = "[";
		for (int32 i = 0; i < dataset.colors.Num(); i++){
			legendColor += "'rgba(" + fColorToFString(dataset.colors[i]) + ")'";
			if ((i+1) < dataset.colors.Num()) {
				legendColor += ",";
			}
		}
		legendColor += "]";
	}
	





	FString datasetsJS = "[";
	datasetsJS += "{ name: '" + dataset.name + "', type : 'pie',";

	switch (textInChart) {
	case EunspacetoolHTMLMenuChartLabelInChartWithPercent::E_none:
		datasetsJS += "label: {show: false}, ";
		break;
	case EunspacetoolHTMLMenuChartLabelInChartWithPercent::E_valAndName:
		datasetsJS += "label: {show: true, formatter(param) {return param.name + ' (' + param.value + ')'; }}, ";
		break;
	case EunspacetoolHTMLMenuChartLabelInChartWithPercent::E_perAndName:
		datasetsJS += "label: {show: true, formatter(param) {return param.name + ' (' + param.percent + '%)'; }}, ";
		break;
	case EunspacetoolHTMLMenuChartLabelInChartWithPercent::E_per:
		datasetsJS += "label: {show: true, formatter(param) {return  param.percent + '%'; }}, ";
		break;
	case EunspacetoolHTMLMenuChartLabelInChartWithPercent::E_val:
		datasetsJS += "label: {show: true, formatter(param) {return  param.value ; }}, ";
		break;
	case EunspacetoolHTMLMenuChartLabelInChartWithPercent::E_name:
		datasetsJS += "label: {show: true, formatter(param) {return param.name; }}, ";
		break;
	}



	datasetsJS += " emphasis : {itemStyle: {shadowBlur: 10,shadowOffsetX : 0,shadowColor : 'rgba(0, 0, 0, 0.5)'}}, data:" 
		+ ueStringFloatMapToJSObjectArray(dataset.data) + " }";
	

	datasetsJS += "]";


	FString toolboxMarkJS = "false";
	if (toolboxMark) {
		toolboxMarkJS = "true";
	}
	FString toolboxDataviewJS = "false";
	if (toolboxDataview) {
		toolboxDataviewJS = "true";
	}
	FString toolboxRestoreJS = "false";
	if (toolboxRestore) {
		toolboxRestoreJS = "true";
	}
	FString toolboxSaveJS = "false";
	if (toolboxSave) {
		toolboxSaveJS = "true";
	}

	FString jsCommand = "createPieChart(" + titleJS +
		"," + legendColor +
		"," + legendOrientJS +
		"," + legendPositionJS +
		"," + toolTipJS + 
		"," + datasetsJS + 
		"," + toolboxMarkJS +
		"," + toolboxDataviewJS +
		"," + toolboxRestoreJS +
		"," + toolboxSaveJS + "); ";

	UE_LOG(LogTemp, Warning, TEXT("%s"), *jsCommand);

	ExecuteJavascript(jsCommand);
}


void UunspacetoolHTMLMenu::createScatterChart(
	FunspacetoolHTMLMenuChartTitle title,
	TArray<FString> legendData,
	EunspacetoolHTMLMenuLegendOrient legendOrient,
	EunspacetoolHTMLMenuChartPosition legendPosition,
	TArray<FunspacetoolHTMLMenuScatterChartSeriesData> datasets,
	bool tooltip,
	bool toolboxMark,
	bool toolboxDataview,
	bool toolboxRestore,
	bool toolboxSave) {

	FString titleJS = "{text: '" + title.title + "', subtext : '" + title.subtitle + "', left :";
	switch (title.position) {
	case EunspacetoolHTMLMenuChartPosition::E_left:
		titleJS += "'left'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_center:
		titleJS += "'center'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_right:
		titleJS += "'right'";
		break;
	}
	titleJS += "}";

	FString legendDataJS = ueStringArrayToJSArray(legendData);
	FString legendOrientJS = "";
	FString legendPositionJS = "";

	switch (legendOrient) {
	case EunspacetoolHTMLMenuLegendOrient::E_vertical:
		legendOrientJS = "'vertical'";
		break;
	case EunspacetoolHTMLMenuLegendOrient::E_horizontal:
		legendOrientJS = "'horizontal'";
		break;
	}

	switch (legendPosition) {
	case EunspacetoolHTMLMenuChartPosition::E_left:
		legendPositionJS = "'left'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_center:
		legendPositionJS = "'center'";
		break;
	case EunspacetoolHTMLMenuChartPosition::E_right:
		legendPositionJS = "'right'";
		break;
	}


	FString toolTipJS = "'none'";
	if (tooltip) {
		toolTipJS = "'item'";
	}

	FString datasetsJS = "[";


	FString legendColor = "[";
	for (int32 i = 0; i < datasets.Num(); i++) {

		FString color = "'rgba(" + fColorToFString(datasets[i].color) + ")'";

		datasetsJS += "{ name: '" + datasets[i].name + "', type : 'scatter',lineStyle: " +
			"{color: " + color + "},";

		datasetsJS += "data:" + ueFunspacetoolHTMLMenuChart2dDataArrayToJSArray(datasets[i].data) + " }";

		if ((i + 1) < datasets.Num()) {
			datasetsJS += ",";
			legendColor += color + ",";
		}
		else {
			legendColor += color;
		}


	}
	datasetsJS += "]";
	legendColor += "]";

	FString swapAxisJS = "false";


	FString toolboxMarkJS = "false";
	if (toolboxMark) {
		toolboxMarkJS = "true";
	}
	FString toolboxDataviewJS = "false";
	if (toolboxDataview) {
		toolboxDataviewJS = "true";
	}
	FString toolboxMagictypeJS = "false";
	FString toolboxRestoreJS = "false";
	if (toolboxRestore) {
		toolboxRestoreJS = "true";
	}
	FString toolboxSaveJS = "false";
	if (toolboxSave) {
		toolboxSaveJS = "true";
	}

	FString jsCommand = "createChart(" + titleJS + "," + legendColor +
		"," + legendOrientJS +
		"," + legendPositionJS +
		"," + legendDataJS +
		", " + toolTipJS + ",[]," + datasetsJS + "," + swapAxisJS +
		"," + toolboxMarkJS +
		"," + toolboxDataviewJS +
		"," + toolboxMagictypeJS +
		"," + toolboxRestoreJS +
		"," + toolboxSaveJS + "); ";

	UE_LOG(LogTemp, Warning, TEXT("%s"), *jsCommand);

	ExecuteJavascript(jsCommand);
}

void UunspacetoolHTMLMenu::getSystemType(EunspacetoolHTMLMenuSystem& system){
	#if PLATFORM_WINDOWS
		system = EunspacetoolHTMLMenuSystem::Windows;
		return;
	#endif	
	#if PLATFORM_LINUX
		system = EunspacetoolHTMLMenuSystem::Linux;
		return;
	#endif	
	#if PLATFORM_MAC
		system = EunspacetoolHTMLMenuSystem::Mac;
		return;
	#endif	
}

void UunspacetoolHTMLMenu::stringToBase64String(FString string, FString& base64String) {
	base64String.Empty();
	FTCHARToUTF8 Convert(*string);
	TArray<uint8> bytes;
	bytes.Append(((uint8*)((ANSICHAR*)Convert.Get())), Convert.Length());
	base64String = FBase64::Encode(bytes);
}

void UunspacetoolHTMLMenu::base64StringToString(FString& string, FString base64String) {
	string.Empty();
	TArray<uint8> bytes;
	if (FBase64::Decode(*base64String, bytes)) {
		bytes.Add(0x00);// null-terminator
		char* Data = (char*)bytes.GetData();
		string = FString(UTF8_TO_TCHAR(Data));
	}
}


void UunspacetoolHTMLMenu::bytesToBase64String(TArray<uint8> bytes, FString& base64String) {
	base64String.Empty();
	base64String = FBase64::Encode(bytes);
}

TArray<uint8> UunspacetoolHTMLMenu::base64StringToBytes(FString base64String, bool& success) {
	TArray<uint8> fileData;
	if (base64String.Len() % 2 != 0 || base64String.Len() < 4) {
		success = false;
		return fileData;
	}
	success = FBase64::Decode(*base64String, fileData);
	return fileData;
}

void UunspacetoolHTMLMenu::fileToBase64String(EHTML_DirectoryType directoryType, FString filePath, bool& success, FString& base64String, FString& fileName) {
	base64String.Empty();
	fileName.Empty();
	TArray<uint8> fileData;
	if (!FFileHelper::LoadFileToArray(fileData, *getCleanDirectory(directoryType, filePath))) {
		success = false;
		return;
	}
	base64String = FBase64::Encode(fileData);
	success = true;
}

void UunspacetoolHTMLMenu::base64StringToFile(FString base64String, EHTML_DirectoryType directoryType, FString filePath, bool& success, FString& absolutePath){
	TArray<uint8> bytes = base64StringToBytes(base64String, success);
	if (!success)
		return;

	absolutePath = getCleanDirectory(directoryType, filePath);
	if (!FFileHelper::SaveArrayToFile(bytes, *absolutePath)) {
		success = false;
		return;
	}
	success = true;
}

FString UunspacetoolHTMLMenu::getCleanDirectory(EHTML_DirectoryType directoryType, FString filePath) {
	if (directoryType == EHTML_DirectoryType::E_ad) {
		return FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		return FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}
}

void UunspacetoolHTMLMenu::LoadURL(FString NewURL)
{
	if (unspacetoolHTMLMenu.IsValid())
	{

		unspacetoolHTMLMenu->BindUObject("uecom", this, true);
		unspacetoolHTMLMenu->LoadURL(NewURL);
	}
}

void UunspacetoolHTMLMenu::LoadString(FString Contents, FString DummyURL)
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("LoadString 1"));*/

	if (unspacetoolHTMLMenu.IsValid())
	{
		unspacetoolHTMLMenu->BindUObject("uecom", this, true);
		unspacetoolHTMLMenu->LoadString(Contents, DummyURL);
	}
}

void UunspacetoolHTMLMenu::ExecuteJavascript(const FString& ScriptText)
{
	
	//UE_LOG(LogTemp, Warning, TEXT("ExecuteJavascript:%s"), *ScriptText);
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, TEXT("ExecuteJavascript 1"));
	if (this->IsValidLowLevel() && unspacetoolHTMLMenu.IsValid())
	{
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, ScriptText);*/
		return unspacetoolHTMLMenu->ExecuteJavascript(ScriptText);
	}
}

FText UunspacetoolHTMLMenu::GetTitleText() const
{
	if (unspacetoolHTMLMenu.IsValid())
	{
		return unspacetoolHTMLMenu->GetTitleText();
	}

	return FText::GetEmpty();
}

FString UunspacetoolHTMLMenu::GetUrl() const
{
	if (unspacetoolHTMLMenu.IsValid())
	{
		return unspacetoolHTMLMenu->GetUrl();
	}

	return FString();
}

void UunspacetoolHTMLMenu::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	unspacetoolHTMLMenu.Reset();
	//UE_LOG(LogTemp, Warning, TEXT("ReleaseSlateResources"));

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, TEXT("bReleaseChildren"));*/

	//if (bReleaseChildren && websocketServerThread != nullptr) {
	//	websocketServerThread->stopThread();
	//	websocketServerThread = nullptr;
	//}

}

TSharedRef<SWidget> UunspacetoolHTMLMenu::RebuildWidget()
{
	//UE_LOG(LogTemp, Warning, TEXT("RebuildWidget"));

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, TEXT("RebuildWidget"));*/

	if (IsDesignTime())
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Web Browser", "Web Browser"))
			];
	}
	else
	{
		FCreateBrowserWindowSettings browserSettings;
		browserSettings.bUseTransparency = transparentBackground;
		browserSettings.BrowserFrameRate = maxMenuFPS;
		browserSettings.bShowErrorMessage = true;
		browserSettings.InitialURL = TEXT("about:blank");
		
#if UE_BUILD_DEBUG | UE_BUILD_DEVELOPMENT
		IWebBrowserModule::Get().GetSingleton()->SetDevToolsShortcutEnabled(allowDevTools);
#else
		IWebBrowserModule::Get().GetSingleton()->SetDevToolsShortcutEnabled(false);
#endif
		if (BrowserWindow.IsValid() == false) {
			BrowserWindow = IWebBrowserModule::Get().GetSingleton()->CreateBrowserWindow(browserSettings);
		}

		
		unspacetoolHTMLMenu = SNew(SWebBrowser, BrowserWindow)
			.InitialURL(TEXT("about:blank"))
			.ShowControls(false)
			.ShowErrorMessage(true)
			.SupportsTransparency(transparentBackground)
			.SupportsThumbMouseButtonNavigation(false)
			.ShowInitialThrobber(false)
			.OnCreateWindow(BrowserWindow->OnCreateWindow())
			.OnBeforePopup(BrowserWindow->OnBeforePopup())
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnUrlChanged));

		if (!BrowserWindow->OnBeforePopup().IsBound()) {
			BrowserWindow->OnBeforePopup().BindUObject(this, &UunspacetoolHTMLMenu::HandleOnBeforePopup);
		}

		if (!BrowserWindow->OnCreateWindow().IsBound()) {
			BrowserWindow->OnCreateWindow().BindUObject(this, &UunspacetoolHTMLMenu::HandleCreateWindow);
		}
		
		return unspacetoolHTMLMenu.ToSharedRef();
	}
}



void UunspacetoolHTMLMenu::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, TEXT("SynchronizeProperties"));
	//if (unspacetoolHTMLMenu.IsValid())
	//{

	//}
}

void UunspacetoolHTMLMenu::handleWebsocketData(FString& s) {
	//UE_LOG(LogTemp, Display, TEXT("URLChange1:%s"), *s);
	if (s.Contains("#") == false) {
		return;
	}

	//touch events
	if (s.Contains("#touch")) {
		TArray<FString> args;
		s.ParseIntoArray(args, TEXT("'();:"), true);
		if (args.Num() < 4) {
			return;
		}
		if (args[1].Equals("javascriptmouseposition")) {
			if (*args[2] != nullptr && *args[3] != nullptr && FCString::IsNumeric(*args[2]) && FCString::IsNumeric(*args[3]))
				javascriptMousePosition(FCString::Atof(*args[2]), FCString::Atof(*args[3]));
			return;
		}

		if (args[1].Equals("triggerue4eventmousedown") && args.Num() == 5) {
			if (*args[2] != nullptr && *args[3] != nullptr && *args[4] != nullptr && FCString::IsNumeric(*args[2]) && FCString::IsNumeric(*args[3]) && FCString::IsNumeric(*args[4]))
				triggerUE4EventMouseDown(FCString::Atof(*args[2]), FCString::Atof(*args[3]), FCString::Atoi(*args[4]));
			return;
		}

		if (args[1].Equals("triggerue4eventmouseup") && args.Num() == 5) {
			if (*args[2] != nullptr && *args[3] != nullptr && *args[4] != nullptr && FCString::IsNumeric(*args[2]) && FCString::IsNumeric(*args[3]) && FCString::IsNumeric(*args[4]))
				triggerUE4EventMouseUp(FCString::Atof(*args[2]), FCString::Atof(*args[3]), FCString::Atoi(*args[4]));
			return;
		}
		return;
	}

	//javascript event
	if (s.Contains("#js")) {
		FString left;
		FString right;
		s.Split("#js", &left, &right);
		//UE_LOG(LogTemp, Display, TEXT("URLChange2:%s"), *left);
		//UE_LOG(LogTemp, Display, TEXT("URLChange3:%s"), *right);
		s = right;//base64Decode(right);

		//UE_LOG(LogTemp, Display, TEXT("URLChange4:%s"), *s);
		if (s.Contains("UE4unspacetoolHTMLMenuUE4ConnectorOnUrlChange") == false) {
			return;
		}

		TArray<FString> args;
		s.ParseIntoArray(args, TEXT("'();:"), true);
		if (args.Num() < 3) {
			return;
		}
		if (args[1].Equals("triggerue4eventwithtag")) {
			switch (args.Num()) {
			case 3:
				triggerUE4EventWithTag(args[2], "", "", "", "");
				break;
			case 4:
				triggerUE4EventWithTag(args[2], args[3], "", "", "");
				break;
			case 5:
				triggerUE4EventWithTag(args[2], args[3], args[4], "", "");
				break;
			case 6:
				triggerUE4EventWithTag(args[2], args[3], args[4], args[5], "");
				break;
			case 7:
				triggerUE4EventWithTag(args[2], args[3], args[4], args[5], args[6]);
				break;
			}
			return;
		}


		if (args[1].Equals("triggerue4eventwithblank")) {
			if (args.Num() > 3) {
				triggerUE4EventWithBlank(args[2], args[3]);
			}
			else {
				triggerUE4EventWithBlank(args[2], "");
			}
			return;
		}

		if (args[1].Equals("triggerue4eventwithmap")) {
			if (args.Num() > 3) {
				triggerUE4EventWithMap(args[2], args[3]);
			}
			else {
				triggerUE4EventWithMap(args[2], "");
			}
			return;
		}

		if (args[1].Equals("triggerue4eventwithjson")) {
			if (args.Num() > 3) {
				triggerUE4EventWithJSON(args[2], args[3]);
			}
			else {
				triggerUE4EventWithJSON(args[2], "");
			}
			return;
		}
		return;
	}

	//graphics
	if (s.Contains("#ue4getgraphics")) {
		javascriptGetGraphics();
		return;
	}

	if (s.Contains("#ue4applyandsavegraphics")) {
		TArray<FString> args;
		s.ParseIntoArray(args, TEXT("'();:"), true);
		if (args.Num() < 6) {
			return;
		}

		javascriptApplyAndSaveGraphics(args[1], FCString::Atoi(*args[2]), FCString::Atoi(*args[3]), FCString::Atoi(*args[4]), FCString::Atof(*args[5]));
		return;
	}




	//console
	if (s.Contains("#console")) {
		TArray<FString> args;
		s.ParseIntoArray(args, TEXT("'();:"), true);
		if (args.Num() < 2) {
			return;
		}
		javascriptConsoleCommand(args[1]);
		return;
	}

	//end game
	if (s.Contains("#ue4quit")) {
		javascriptQuitGame();
		return;
	}

	//change level
	if (s.Contains("#ue4travel")) {
		TArray<FString> args;
		s.ParseIntoArray(args, TEXT("'();:"), true);
		if (args.Num() < 2) {
			return;
		}
		javascriptClientTravel(args[1]);
		return;
	}

}




TArray<int32> UunspacetoolHTMLMenu::checkIpAndPort(FString IP, int32 portMin, int32 portMax) {


	//simple ip check
	TArray<FString> ipStringArray;
	int32 lineCount = IP.ParseIntoArray(ipStringArray, TEXT("."), true);
	if (lineCount != 4) {
		UE_LOG(LogTemp, Error, TEXT("Invalid IP:%s"), *IP);
		return TArray<int32>();
	}
	TArray<int32> ipArray = { FCString::Atoi(*ipStringArray[0]), FCString::Atoi(*ipStringArray[1]), FCString::Atoi(*ipStringArray[2]) ,FCString::Atoi(*ipStringArray[3]) };
	if ((ipArray[0] < 0 || ipArray[0] > 255) || (ipArray[1] < 0 || ipArray[1] > 255) || (ipArray[2] < 0 || ipArray[2] > 255) || (ipArray[3] < 0 || ipArray[3] > 255)) {
		UE_LOG(LogTemp, Error, TEXT("Invalid IP:%s"), *IP);
		return TArray<int32>();
	}

	//simple port check
	if (portMin < 0 || portMin > 65535) {
		UE_LOG(LogTemp, Error, TEXT("Invalid port:%i"), portMin);
		return TArray<int32>();
	}
	if (portMax < 0 || portMax > 65535) {
		UE_LOG(LogTemp, Error, TEXT("Invalid port:%i"), portMax);
		return TArray<int32>();
	}
	return ipArray;
}

void UunspacetoolHTMLMenu::HandleOnUrlChanged(const FText& InText) {
	FString url = InText.ToString();
	if (lastUrl.IsEmpty()) {
		lastUrl = url;
		return;
	}
	if (lastUrl.Equals(url)) {
		return;
	}
	lastUrl = url;

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Purple, *url);
	
}

FString UunspacetoolHTMLMenu::base64Decode(FString text) {
	TArray<uint8> fileData;
	FBase64::Decode(*text, fileData);
	fileData.Add('\0');
	return FString(UTF8_TO_TCHAR(fileData.GetData()));
}


bool UunspacetoolHTMLMenu::HandleCreateWindow(const TWeakPtr<IWebBrowserWindow>& NewBrowserWindow, const TWeakPtr<IWebBrowserPopupFeatures>& PopupFeatures){

	if (NewBrowserWindow.IsValid() == false) {
		return false;
	}


	TSharedPtr<SWindow> mainWindow = FSlateApplication::Get().FindWidgetWindow(unspacetoolHTMLMenu.ToSharedRef());
	if (!mainWindow.IsValid())
		return false;



	TSharedRef<SWindow> devToolsWindow =
		SNew(SWindow)
		.Title(FText::GetEmpty())
		.ScreenPosition(FVector2D(0, 0))
		.ClientSize(FVector2D(800,800))
		.AutoCenter(EAutoCenter::None)
		.SizingRule(ESizingRule::UserSized)
		.SupportsMinimize(true)
		.SupportsMaximize(true)
		.CreateTitleBar(true)
		.HasCloseButton(true)
		.IsInitiallyMaximized(false)
		.LayoutBorder(FMargin(0));

	TSharedPtr<SWebBrowser> WebBrowser;
	devToolsWindow->SetContent(
		SNew(SBorder)
		.Padding(0)
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SAssignNew(WebBrowser, SWebBrowser, NewBrowserWindow.Pin())
			.ShowControls(false)
		.ShowAddressBar(false)
		]);


	FSlateApplication::Get().AddWindow(devToolsWindow);
	devToolsWindow->BringToFront();

	return true;
}

bool UunspacetoolHTMLMenu::HandleOnBeforePopup(FString URL, FString Frame){
	//UE_LOG(LogTemp, Warning, TEXT("HandleOnBeforePopup: %s"), *URL);

	if (URL.Equals("chrome-devtools://devtools/devtools.html")) {
		return false;
	}
	if (allowPopups) {
		return false;
	}
	return true;
}

#if WITH_EDITOR

const FText UunspacetoolHTMLMenu::GetPaletteCategory()
{
	return LOCTEXT("HTML Menu", "HTML Menu");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

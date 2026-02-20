// Copyright 2018-2020 David Romanski (Socke). All Rights Reserved.
#pragma once

#include "unspacetoolHTMLMenuMain.h"
#include "JavascriptBean.generated.h"


class UunspacetoolHTMLMenu;

USTRUCT(BlueprintType)
struct FStringTMap
{
	GENERATED_USTRUCT_BODY()

		TMap<FString, FString> stringMap;
};

/**
*
*/
UCLASS(Blueprintable, BlueprintType)
class UJavascriptBean : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
		FString InaccessibleUProperty;




	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FJavascriptEventTag, const FString, ID, const FString, ClassName, const FString, Value, const TArray<FString>&, args);
	UFUNCTION()
		void JavascriptEventTag(const FString ID, const FString ClassName, const FString Value, const TArray<FString>& args);
	UPROPERTY(BlueprintAssignable, Category = "HTML Menu|Events|JavascriptEventTag")
		FJavascriptEventTag onJavascriptEventTag;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJavascriptEventBlank, const TArray<FString>&, args);
	UFUNCTION()
		void JavascriptEventBlank(const TArray<FString>& args);
	UPROPERTY(BlueprintAssignable, Category = "HTML Menu|Events|JavascriptEventBlank")
		FJavascriptEventBlank onJavascriptEventBlank;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJavascriptEventMap, const FStringTMap, map);
	UFUNCTION()
		void JavascriptEventMap(const FStringTMap map);
	UPROPERTY(BlueprintAssignable, Category = "HTML Menu|Events|JavascriptEventMap")
		FJavascriptEventMap onJavascriptEventMap;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJavascriptEventJSON, const FString, json);
	UFUNCTION()
		void JavascriptEventJSON(const FString json);
	UPROPERTY(BlueprintAssignable, Category = "HTML Menu|Events|JavascriptEventJSON")
		FJavascriptEventJSON onJavascriptEventJSON;


	/**
	* UE4 does not support maps in events. Therefore, this intermediate step.
	*/
	UFUNCTION(BlueprintCallable, Category = "HTML Menu")
		static TMap<FString, FString> toStringMap(FStringTMap map);


	void setBrowser(UunspacetoolHTMLMenu* browser);
private:
	UunspacetoolHTMLMenu* browser;
};

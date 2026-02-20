// Copyright 2018-2020 David Romanski (Socke). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "unspacetoolHTMLMenuMain.h"
#include "Components/Widget.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Runtime/Core/Public/Misc/SecureHash.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "Runtime/Core/Public/HAL/FileManager.h"
#include "Runtime/Engine/Classes/Components/ApplicationLifecycleComponent.h"
#include "IWebBrowserWindow.h"
#include "unspacetoolHTMLMenu.generated.h"

class UJavascriptBean;
class UMaterial;


UENUM(BlueprintType)
enum class EHTML_JavascriptFunction : uint8
{
	E_Up 			UMETA(DisplayName = "keyUP"),
	E_Down			UMETA(DisplayName = "keyDown"),
	E_Left 			UMETA(DisplayName = "keyLeft"),
	E_Right			UMETA(DisplayName = "keyRight"),
	E_Next			UMETA(DisplayName = "nexElement"),
	E_Previous		UMETA(DisplayName = "previousElement"),
	E_Choose		UMETA(DisplayName = "click")
};


UENUM(BlueprintType)
enum class EHTML_FileType : uint8
{
	E_pdf	UMETA(DisplayName = "PDF")
};


UENUM(BlueprintType)
enum class EHTML_DirectoryType : uint8
{
	E_gd	UMETA(DisplayName = "Game directory"),
	E_ad 	UMETA(DisplayName = "Absolute directory")
};

UENUM(BlueprintType)
enum class EunspacetoolHTMLMenuSystem : uint8
{
	Android,
	IOS,
	Windows,
	Linux,
	Mac
};


UENUM(BlueprintType)
enum class EunspacetoolHTMLMenuChartTooltip: uint8
{
	E_none	UMETA(DisplayName = "None"),
	E_item 	UMETA(DisplayName = "Item"),
	E_axis	UMETA(DisplayName = "Axis")
};

UENUM(BlueprintType)
enum class EunspacetoolHTMLMenuChartType : uint8
{
	E_line	UMETA(DisplayName = "Line"),
	E_bar 	UMETA(DisplayName = "Bar")
};

UENUM(BlueprintType)
enum class EunspacetoolHTMLMenuLegendOrient : uint8
{
	E_vertical	UMETA(DisplayName = "Vertical"),
	E_horizontal 	UMETA(DisplayName = "Horizontal")
};

UENUM(BlueprintType)
enum class EunspacetoolHTMLMenuChartPosition : uint8
{
	E_left		UMETA(DisplayName = "left"),
	E_center 	UMETA(DisplayName = "center"),
	E_right 	UMETA(DisplayName = "right")
};




UENUM(BlueprintType)
enum class EunspacetoolHTMLMenuChartLabelInChart: uint8
{
	E_none	UMETA(DisplayName = "None"),
	E_valAndName	UMETA(DisplayName = "Value and Name"),
	E_val 	UMETA(DisplayName = "Value"),
	E_name 	UMETA(DisplayName = "Name")
};


UENUM(BlueprintType)
enum class EunspacetoolHTMLMenuChartLabelInChartWithPercent : uint8
{
	E_none	UMETA(DisplayName = "None"),
	E_valAndName	UMETA(DisplayName = "Name and Value"),
	E_perAndName	UMETA(DisplayName = "Name and Percent"),
	E_val 	UMETA(DisplayName = "Value"),
	E_per 	UMETA(DisplayName = "Percent"),
	E_name 	UMETA(DisplayName = "Name")
};


USTRUCT(BlueprintType)
struct FunspacetoolHTMLMenuChartTitle
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
	FString title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
	FString subtitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
	EunspacetoolHTMLMenuChartPosition position = EunspacetoolHTMLMenuChartPosition::E_left;
};


USTRUCT(BlueprintType)
struct FunspacetoolHTMLMenuChartSeriesData
{
	GENERATED_USTRUCT_BODY()

	/**
	*xxxxxxxxxxxxxxxxx.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		TArray<float> data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		FColor lineColor = FColor(128,128,128,1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		FColor backgroundColor = FColor(128, 128, 128, 1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		bool fillBackground = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		EunspacetoolHTMLMenuChartLabelInChart textInChart = EunspacetoolHTMLMenuChartLabelInChart::E_none;
};


USTRUCT(BlueprintType)
struct FunspacetoolHTMLMenuPieChartSeriesData
{
	GENERATED_USTRUCT_BODY()

		/**
		*xxxxxxxxxxxxxxxxx.*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		TMap<FString,float> data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		TArray<FColor> colors;
};


USTRUCT(BlueprintType)
struct FunspacetoolHTMLMenuChart2dData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		float x = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		float y = 0;
};

USTRUCT(BlueprintType)
struct FunspacetoolHTMLMenuScatterChartSeriesData
{
	GENERATED_USTRUCT_BODY()

		/**
		*xxxxxxxxxxxxxxxxx.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		TArray<FunspacetoolHTMLMenuChart2dData> data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTML Menu")
		FColor color = FColor(128, 128, 128, 1);
};




UCLASS()
class UunspacetoolHTMLMenu : public UWidget
{
	GENERATED_UCLASS_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUrlChanged, const FText&, Text);
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeforePopup, FString, URL, FString, Frame);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FJavascriptEventMouseDown, const float, X, const float, Y, const int32, button);
	UFUNCTION()
		void JavascriptEventMouseDown(const float X, const float Y, const int32 button);
	UPROPERTY(BlueprintAssignable, Category = "HTML Menu|Events|MouseDown")
		FJavascriptEventMouseDown onJavascriptEventMouseDown;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FJavascriptEventMouseUp, const float, X, const float, Y, const int32, button);
	UFUNCTION()
		void JavascriptEventMouseUp(const float X, const float Y, const int32 button);
	UPROPERTY(BlueprintAssignable, Category = "HTML Menu|Events|MouseUp")
		FJavascriptEventMouseUp onJavascriptEventMouseUp;
	/**
	* Register Javascript event so that it can be fired in blueprints.
	*
	* @param nameOfJavascriptFunction If you call an event in Javascript with triggerUE4Event('myJavascriptEvent') then you have to put myJavascriptEvent in this field.
	* @param eventTarget This is the target for the Blueprint Event.
	*/
	UFUNCTION(BlueprintCallable, Category = "HTML Menu")
		void registerJavascriptEvent(const FString nameOfJavascriptFunction, UJavascriptBean*& eventTarget);

	UFUNCTION()
		void triggerUE4EventWithTag(FString javascriptFunctionsName, FString ID, FString className, FString value, FString arguments);
	UFUNCTION()
		void triggerUE4EventWithBlank(FString javascriptFunctionsName, FString arguments);
	UFUNCTION()
		void triggerUE4EventWithMap(FString javascriptFunctionsName, FString arguments);
	UFUNCTION()
		void triggerUE4EventWithJSON(FString javascriptFunctionsName, FString json);
	UFUNCTION()
		void triggerUE4EventMouseDown(float x, float y, int32 button);
	UFUNCTION()
		void triggerUE4EventMouseUp(float x, float y, int32 button);

	/*Javascript functions start*/

	UFUNCTION()
		void javascriptConsoleCommand(FString command);
	UFUNCTION()
		void javascriptQuitGame();
	UFUNCTION()
		void javascriptApplyAndSaveGraphics(FString resolution, int32 windowMode, int32 antiAliasing, int32 shadows, float resolutionScale);
	UFUNCTION()
		void javascriptGetGraphics();
	UFUNCTION()
		void javascriptClientTravel(FString mapOrAdress);
	UFUNCTION()
		void javascriptMousePosition(float x, float y);

	/*Javascript functions end*/


	UFUNCTION(BlueprintCallable, Category = "HTML Menu")
		void getHTMLMouseCursorPosition(float& x, float& y);

	UPROPERTY(EditAnywhere, Category = "HTML Menu settings")
		bool transparentBackground = true;

	UPROPERTY(EditAnywhere, Category = "HTML Menu settings")
		int32 maxMenuFPS = 60;

	/*
	For security reasons only in debug and development mode
	*/
	UPROPERTY(EditAnywhere, Category = "HTML Menu settings")
		bool allowDevTools = false;

	UPROPERTY(EditAnywhere, Category = "HTML Menu settings")
		bool allowPopups = false;

	/**
	* Creates a new HTML file. All. js, css files and images will be inserted. It's all in a single HTML file. This file works even if the game is packed (. pak file). Deletes \t, \n, \r characters. Please do not use \\ comments in your css,js and html.
	*
	* @param localSourceHTML_File The HTML to convert
	* @param relativeTargetHTML_Directory The new HTML file is created in this directory
	* @param overwriteExistingFile Overwrite existing file
	* @param onlyInEditor true = Runs only in the editor. This node jumps then to return in a packed project.
	*/
	UFUNCTION(BlueprintCallable, Category = "HTML Menu")
		UunspacetoolHTMLMenu* generateHTML_File(FString localSourceHTML_File, FString relativeTargetHTML_Directory, bool overwriteExistingFile, bool onlyInEditor=true);

	/**
	*
	*
	* @param
	*/
	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Javascript")
		void triggerKeyAction(EHTML_JavascriptFunction function);

	/**
	* Loads an HTML file.
	*
	* @param htmlFile HTML File
	* @param removeLineBreaks If the file contains line breaks, they must be removed to load the page.
	*/
	UFUNCTION(BlueprintCallable, Category = "HTML Menu")
		UunspacetoolHTMLMenu* loadHTML_File(FString htmlFile = FString("Content/"), bool removeLineBreaks = false);

	UFUNCTION(BlueprintCallable, Category = "HTML Menu|PDF")
		static void LoadAllFilesFromDirectory(EHTML_DirectoryType directoryType, EHTML_FileType fileType, FString dirPath, int32& count, TArray<FString>& files, TArray<FString>& filePaths);

	UFUNCTION(BlueprintCallable, Category = "HTML Menu|PDF")
		bool LoadPDF_FileIntoViewer(EHTML_DirectoryType directoryType, FString filePathWithFilename);

	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Chart")
		bool loadChartLib(FString chartLib = FString("Content/chart.html"));

	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Chart")
		void createChart(EunspacetoolHTMLMenuChartType type,
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
			bool toolboxSave);

	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Chart")
		void createPieChart(
			FunspacetoolHTMLMenuChartTitle title,
			EunspacetoolHTMLMenuLegendOrient legendOrient,
			EunspacetoolHTMLMenuChartPosition legendPosition,
			FunspacetoolHTMLMenuPieChartSeriesData dataset,
			EunspacetoolHTMLMenuChartLabelInChartWithPercent textInChart,
			bool tooltip,
			bool toolboxMark,
			bool toolboxDataview,
			bool toolboxRestore,
			bool toolboxSave);

	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Chart")
		void createScatterChart(
			FunspacetoolHTMLMenuChartTitle title,
			TArray<FString> legendData,
			EunspacetoolHTMLMenuLegendOrient legendOrient,
			EunspacetoolHTMLMenuChartPosition legendPosition,
			TArray<FunspacetoolHTMLMenuScatterChartSeriesData> datasets,
			bool tooltip,
			bool toolboxMark,
			bool toolboxDataview,
			bool toolboxRestore,
			bool toolboxSave);


	UFUNCTION(BlueprintCallable, Category = "HTML Menu")
		static void getSystemType(EunspacetoolHTMLMenuSystem& system);


	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Base64")
		static void stringToBase64String(FString string, FString& base64String);
	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Base64")
		static void base64StringToString(FString& string, FString base64String);
	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Base64")
		static void bytesToBase64String(TArray<uint8> bytes, FString& base64String);
	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Base64")
		static TArray<uint8> base64StringToBytes(FString base64String, bool& success);
	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Base64")
		static void fileToBase64String(EHTML_DirectoryType directoryType, FString filePath, bool& success, FString& base64String, FString& fileName);
	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Base64")
		static void base64StringToFile(FString base64String, EHTML_DirectoryType directoryType, FString filePath, bool& success, FString& absolutePath);


	/**
	* Load the specified URL
	*
	* @param NewURL New URL to load
	*/
	UFUNCTION(BlueprintCallable, Category = "HTML Menu")
		void LoadURL(FString NewURL);

	/**
	* Load a string as data to create a web page
	*
	* @param Contents String to load
	* @param DummyURL Dummy URL for the page
	*/
	UFUNCTION(BlueprintCallable, Category = "Web Browser")
		void LoadString(FString Contents, FString DummyURL);

	/**
	* Executes a JavaScript string in the context of the web page
	*
	* @param ScriptText JavaScript string to execute
	*/
	UFUNCTION(BlueprintCallable, Category = "HTML Menu|Javascript")
		void ExecuteJavascript(const FString& ScriptText);

	/**
	* Get the current title of the web page
	*/
	UFUNCTION(BlueprintCallable, Category = "Web Browser")
		FText GetTitleText() const;

	/**
	* Gets the currently loaded URL.
	*
	* @return The URL, or empty string if no document is loaded.
	*/
	UFUNCTION(BlueprintCallable, Category = "Web Browser")
		FString GetUrl() const;

	/** Called when the Url changes. */
	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
		FOnUrlChanged OnUrlChanged;

	bool HandleCreateWindow(const TWeakPtr<IWebBrowserWindow>& NewBrowserWindow, const TWeakPtr<IWebBrowserPopupFeatures>& PopupFeatures);

	void setBrowserWindow(TSharedPtr<IWebBrowserWindow> NewBrowserWindow) {
		BrowserWindow = NewBrowserWindow;
	};

	/** Called when a popup is about to spawn. */
	//UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
	//	FOnBeforePopup OnBeforePopup;


	TSharedPtr<IWebBrowserWindow> BrowserWindow;


	/**
	* This can be used to restart the Browser -> UE4 connection on smartphones. Works only on IOS and Android.
	*/
	UFUNCTION(BlueprintCallable, Category = "HTML Menu")
		void reconnectMobile();

	static TArray<int32> checkIpAndPort(FString IP, int32 portMin, int32 portMax);

	FString base64Decode(FString text);
	void handleWebsocketData(FString& data);

	UFUNCTION()
		void appReactivtedDelegate();
	UFUNCTION()
		void appEnterBackgroundDelegate();


	int32 lastWebsocketPort = 0;

	void setWebsocketPort(int32 port);
	int32 getWebsocketPort();

public:

	static FString getCleanDirectory(EHTML_DirectoryType directoryType, FString filePath);

	//~ Begin UWidget interface
	virtual void SynchronizeProperties() override;
	// End UWidget interface

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	UMaterial* GetDefaultMaterial() const;
protected:
	/** Should the browser window support transparency. */
	UPROPERTY(EditAnywhere, Category = Appearance)
		bool bSupportsTransparency;

	void HandleOnUrlChanged(const FText& Text);

	TSharedPtr<class SWebBrowser> unspacetoolHTMLMenu;

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
	bool HandleOnBeforePopup(FString URL, FString Frame);

private:
	FString lastUrl;
	FString widgetID;
	static TMap<FString, UJavascriptBean*> javascriptBeans;

	void createAndMinifyunspacetoolHTMLMenu(FString sourceHTML_File, FString targetDir);
	FString replaceCSSImages(FString content, FString cssURL);
	FString replaceCSSFonts(FString content, FString cssURL);
	FString replaceHTMLImages(FString content, FString htmlURL);
	FString removeJavascriptComments(FString content);

	float htmlMouseCursorPositionX;
	float htmlMouseCursorPositionY;

	UMaterial* DefaultMaterial;
	
	AActor* mobileActivityActor = nullptr;

	FString joinFloatArray(TArray<float> data, FString Separator);
	FString fColorToFString(FColor color);
	FString ueStringArrayToJSArray(TArray<FString> ueArray);
	FString ueFloatArrayToJSArray(TArray<float> ueArray);
	FString ueFunspacetoolHTMLMenuChart2dDataArrayToJSArray(TArray<FunspacetoolHTMLMenuChart2dData> ueArray);
	FString ueStringFloatMapToJSObjectArray(TMap<FString, float> ueMap);

};







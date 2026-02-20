// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/*#include <DeclarativeSyntaxSupport.h>*/ 
#include "Widgets/SCompoundWidget.h" //Added for Slate Macro Support (SLATE_BEGIN_ARGS & SLATE_ARGUMENT)
#include <IContentBrowserSingleton.h>
#include <ContentBrowserModule.h>
#include <Editor/ContentBrowser/Private/SPathPicker.h>
#include "UObject/NoExportTypes.h"
#include "AssetToolsModule.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"




// directory selection dialog (slate)

class SDirectoryDialog : public SCompoundWidget
{

public:
   

    SLATE_BEGIN_ARGS(SDirectoryDialog) {}

    SLATE_END_ARGS()

public:
    SDirectoryDialog() {}
    virtual ~SDirectoryDialog() {}

    virtual void Construct(const FArguments& InArgs, const FString& InBasePath)
    {
        //assetutils::ValidPath basePath(InBasePath);

        FString basePath = InBasePath;

        FPathPickerConfig config;
        config.DefaultPath = basePath;
        config.OnPathSelected = FOnPathSelected::CreateRaw(this, &SDirectoryDialog::OnPathSelected);

        FContentBrowserModule& contentBrowserModule
            = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
        TSharedPtr<SPathPicker> pathPicker = StaticCastSharedRef<SPathPicker>(
            contentBrowserModule.Get().CreatePathPicker(config));

        TSharedRef<SHorizontalBox> buttonsBox = SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Bottom)
            .Padding(4.0f, 3.0f)
            [
                SNew(SButton)
                    .Text(FText::FromString(TEXT("OK")))
                    .ContentPadding(FMargin(8.0f, 2.0f))
                    .IsEnabled(this, &SDirectoryDialog::IsConfirmButtonEnabled)
                    .OnClicked(this, &SDirectoryDialog::OnConfirmButtonClicked)
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Bottom)
            .Padding(4.0f, 3.0f)
            [
                SNew(SButton)
                    .Text(FText::FromString(TEXT("Cancel")))
                    .ContentPadding(FMargin(8.0f, 2.0f))
                    .OnClicked(this, &SDirectoryDialog::OnCancelButtonClicked)
            ];

        TSharedRef<SVerticalBox> mainBox = SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            .Padding(0.0f, 0.0f, 0.0f, 4.0f)
            [
                pathPicker.ToSharedRef()
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Fill)
            .Padding(0.0f)
            [
                buttonsBox
            ];

        ChildSlot
            [
                mainBox
            ];
    }

public: // public methods
    bool HasValidResult() const { return ConfirmedFlag && (!SelectedPath.IsEmpty()); }
    const FString& GetPath() const { return SelectedPath; }

private: // internal methods: utilities
    void CloseDialog()
    {
        TSharedPtr<SWindow> window = FSlateApplication::Get().FindWidgetWindow(AsShared());
        if (window.IsValid())
        {
            window->RequestDestroyWindow();
        }
    }

private: // internal methods: delegates
    bool IsConfirmButtonEnabled() const { return true; }

    FReply OnConfirmButtonClicked()
    {
        CloseDialog();
        ConfirmedFlag = true;
        return FReply::Handled();
    }

    FReply OnCancelButtonClicked()
    {
        CloseDialog();
        ConfirmedFlag = false;
        return FReply::Handled();
    }

    void OnPathSelected(const FString& InCurrentPath) { SelectedPath = InCurrentPath; }

private: // internal properties
    FString SelectedPath;
    bool ConfirmedFlag = false;
};


namespace assetutils
{
    class ValidPath
    {
    public:
        ValidPath(const FString& InPath)
        {
          /*  if (UEditorActorSubsystem::DoesDirectoryExist(InPath))
            {
                Path = InPath;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("assetutils::ValidPath()  InPath(%s) is not exist, \"/Game\" is used instead."), *InPath);
                Path = TEXT("/Game");
            }*/
        }
    public:
        const FString& Get() const { return Path; }
    private: // internal properties
        FString Path;
    }; // class ValidPath

} // namespace assetutils
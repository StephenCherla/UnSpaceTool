// Copyright Epic Games, Inc. All Rights Reserved.

#include "unspace_tool.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"
#include "UnspaceToolStyle.h"
#include "Widgets/Docking/SDockTab.h"
#include "LevelEditor.h"
#include "EUSTEditor.h"


IMPLEMENT_MODULE(Funspace_toolModule, unspace_tool)
#define LOCTEXT_NAMESPACE "Funspace_toolModule"

const FName Funspace_toolModule::UnspaceToolTabID = TEXT("UnSpaceToolTab");

void Funspace_toolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FUnspaceToolStyle::Initialize();

	// Register OnPostEngineInit delegate.
	// OnPostEngineInitDelegateHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &Funspace_toolModule::OnPostEngineInit);

	// Create and initialize Editor object.
	Editor = NewObject<UEUSTEditor>(GetTransientPackage(), UEUSTEditor::StaticClass());
	Editor->Init();

	OnPostEngineInit();
	
	
}

void Funspace_toolModule::ShutdownModule()
{
	if (GIsEditor && !IsRunningCommandlet())
	{
		// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
		// we call this function before unloading the module.

		// Unregister the startup function
		UToolMenus::UnRegisterStartupCallback(this);

		// Unregister all our menu extensions
		UToolMenus::UnregisterOwner(this);

		// Unregister Tab Spawner
		FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UnspaceToolTabID);

		// Cleanup the Editor object.
		Editor = nullptr;

		// Remove OnPostEngineInit delegate
		//if(OnPostEngineInitDelegateHandle.IsValid())
			//FCoreDelegates::OnPostEngineInit.Remove(OnPostEngineInitDelegateHandle);

		FUnspaceToolStyle::Shutdown();
	}
}

void Funspace_toolModule::OnPostEngineInit()
{
	// This function is valid only if no Commandlet or game is running. It also requires Slate Application to be initialized.
	if ((IsRunningCommandlet() == false) && (IsRunningGame() == false) && FSlateApplication::IsInitialized())
	{
		if (FLevelEditorModule* LevelEditor = FModuleManager::LoadModulePtr<FLevelEditorModule>(TEXT("LevelEditor")))
		{
			// Register a function to be called when menu system is initialized
			UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
			this, &Funspace_toolModule::RegisterMenuExtensions));

			RegisterTabSpawner();
		}
		
	}
}

void Funspace_toolModule::RegisterMenuExtensions()
{
	// Use the current object as the owner of the menus
	// This allow us to remove all our custom menus when the 
	// module is unloaded (see ShutdownModule below)
	FToolMenuOwnerScoped OwnerScoped(this);

	// Extend the "Tools" section of the main toolbar
	UToolMenu* ToolsMenu = UToolMenus::Get()->ExtendMenu(
		TEXT("LevelEditor.MainMenu.Tools"));

	FToolMenuSection& ToolSection = ToolsMenu->FindOrAddSection(TEXT("Tools"));

	ToolSection.AddEntry(FToolMenuEntry::InitMenuEntry(
		FName("Unspace Tool"),
		FText::FromString("Unspace Tool"),
		FText::FromString("Virtual Production helper utility"),
		FSlateIcon(FUnspaceToolStyle::Get().GetStyleSetName(), TEXT("Icon.Logo")),
		FUIAction(
			FExecuteAction::CreateLambda([this]()
				{
					this->InvokeEditorSpawn();
				}),
			FCanExecuteAction::CreateLambda([]() { return true; })
		)
	));

	//FExecuteAction::CreateLambda([this]()
	//	{
	//		// Do something when the entry is clicked
	//		this->InvokeEditorSpawn();
	//	})
}

void Funspace_toolModule::RegisterTabSpawner()
{
	// Register Tab Spawner.
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		UnspaceToolTabID,
		FOnSpawnTab::CreateRaw(this, &Funspace_toolModule::SpawnEditor),
		FCanSpawnTab::CreateLambda([this](const FSpawnTabArgs& Args) -> bool { return CanSpawnEditor(); })
	)
		//.SetMenuType(ETabSpawnerMenuType::Hidden)
		.SetAutoGenerateMenuEntry(false);

		/*.SetDisplayName(FText::FromString(TEXT("Unspace Tool")))
		.SetIcon(FSlateIcon(FUnspaceToolStyle::Get().GetStyleSetName(), TEXT("Icon.Logo")));*/
}

void Funspace_toolModule::AddReferencedObjects(FReferenceCollector& Collector)
{
	// Prevent Editor Object from being garbage collected.
	if (Editor)
	{
		Collector.AddReferencedObject(Editor);
	}
}

#if (ENGINE_MAJOR_VERSION == 5)
FString Funspace_toolModule::GetReferencerName() const
{
	return TEXT("unspace_toolModuleGCObject");
}
#endif

bool Funspace_toolModule::CanSpawnEditor()
{
	// Editor can be spawned only when the Editor object say that UI can be created.
	if (Editor && Editor->CanCreateEditorUI())
	{
		return true;
	}
	return false;
}

TSharedRef<SDockTab> Funspace_toolModule::SpawnEditor(const FSpawnTabArgs& Args)
{
	// Spawn the Editor only when we can.
	if (IsValid(Editor) && CanSpawnEditor())
	{
		// Spawn new DockTab and fill it with newly created editor UI.
		TSharedRef<SDockTab> NewTab = SAssignNew(EditorTab, SDockTab)
			.TabRole(ETabRole::NomadTab)
			[
				Editor->CreateEditorUI()
			];

		
		// Tell the Editor Object about newly spawned DockTab, as it will 
		// need it to handle various editor actions.
		Editor->SetEditorTab(NewTab);

		// Return the DockTab to the Global Tab Manager.
		return NewTab;
	}

	// If editor can't be spawned - create an empty tab.
	return SAssignNew(EditorTab, SDockTab).TabRole(ETabRole::NomadTab);
}

bool Funspace_toolModule::IsEditorSpawned()
{
	return FGlobalTabmanager::Get()->FindExistingLiveTab(UnspaceToolTabID).IsValid();
}

void Funspace_toolModule::InvokeEditorSpawn()
{
	FGlobalTabmanager::Get()->TryInvokeTab(UnspaceToolTabID);
}

#undef LOCTEXT_NAMESPACE

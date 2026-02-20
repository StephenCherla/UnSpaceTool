// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "UObject/GCObject.h"

class Funspace_toolModule : public IModuleInterface, public FGCObject
{
private:
	// Editor object.
	class UEUSTEditor* Editor;

	// DockTab reference with the editor.
	TWeakPtr<class SDockTab> EditorTab;

	// Handler for an OnPOstEngineInit delegate.
	FDelegateHandle OnPostEngineInitDelegateHandle;

public:
	const static FName UnspaceToolTabID;


private:

	/**
 * Returns true if the editor can be spawned.
 */
	bool CanSpawnEditor();

	/**
	 * Spawns editor and returns a ref of the DockTab to which the editor
	 * has been pinned.
	 */
	TSharedRef<class SDockTab> SpawnEditor(const FSpawnTabArgs& Args);

	/**
	 * Checks if the editor is spawned.
	 */
	bool IsEditorSpawned();

	/**
	 * Invokes spawning editor from the command.
	 */
	void InvokeEditorSpawn();

protected:

	/**
	 * Run some initializations after the Engine has been initialized.
	 */
	void OnPostEngineInit();

public:

	static Funspace_toolModule& Get()
	{
		return FModuleManager::GetModuleChecked<Funspace_toolModule>(TEXT("UnSpaceTool"));
	}

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void RegisterMenuExtensions();

	void RegisterTabSpawner();

	// FGCObject implementation
	void AddReferencedObjects(FReferenceCollector& Collector) override;
	#if (ENGINE_MAJOR_VERSION == 5)
		FString GetReferencerName() const override;
	#endif


};

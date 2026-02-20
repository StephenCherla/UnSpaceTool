// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/UST_EditorContext_LevelEditor.h"
#include <Editor.h>
#include <LevelEditor.h>
#include <EdMode.h>
#include <EditorViewportClient.h>
#include <LevelEditorViewport.h>
#include <EditorModeManager.h>
#include <EditorModeTools.h>

UUST_EditorContext_LevelEditor::~UUST_EditorContext_LevelEditor()
{
	//GEditor->OnLevelActorAttached().RemoveAll(this);
	//GEditor->OnLevelActorDetached().RemoveAll(this);

	//GEditor->OnActorMoved().RemoveAll(this);

	FLevelEditorModule& levelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	levelEditor.OnActorSelectionChanged().RemoveAll(this);
}

void UUST_EditorContext_LevelEditor::Setup()
{
	GEditor->OnLevelActorAttached().AddUObject(this, &UUST_EditorContext_LevelEditor::HandleOnLevelActorAttached);
	GEditor->OnLevelActorDetached().AddUObject(this, &UUST_EditorContext_LevelEditor::HandleOnLevelActorDetached);

	GEditor->OnActorMoved().AddUObject(this, &UUST_EditorContext_LevelEditor::HandleOnActorMoved);

	FLevelEditorModule& levelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	levelEditor.OnActorSelectionChanged().AddUObject(this, &UUST_EditorContext_LevelEditor::HandleOnActorSelectionChanged);
}

void UUST_EditorContext_LevelEditor::HandleOnActorSelectionChanged(const TArray<UObject*>& NewSelection, bool bForceRefresh)
{
	OnActorSelectionChanged.Broadcast(NewSelection);

	for (UObject* object : NewSelection) {
		bool found = false;
		for (UObject* oldObject : OldActorSelection) {
			if (object == oldObject) {
				found = true;
				OldActorSelection.Remove(object); // object is still selected so we can ignore it!
				break;
			}
		}
		if (!found) {
			OnActorSelected.Broadcast(object);  // new selection so let the world know about it!
		}
	}

	// if there still are old objects they are no longer selected so broadcast it
	if (OldActorSelection.Num() > 0) {
		for (UObject* object : OldActorSelection) {
			OnActorDeselected.Broadcast(object);
		}
	}

	OldActorSelection = NewSelection;
}

void UUST_EditorContext_LevelEditor::HandleOnLevelActorAttached(AActor* InActor, const AActor* InParent)
{
	OnLevelActorAttached.Broadcast(InActor, InParent);
}

void UUST_EditorContext_LevelEditor::HandleOnLevelActorDetached(AActor* InActor, const AActor* InParent)
{
	OnLevelActorDetached.Broadcast(InActor, InParent);
}


void UUST_EditorContext_LevelEditor::HandleOnActorMoved(AActor* InActor)
{
	OnActorMoved.Broadcast(InActor);
}
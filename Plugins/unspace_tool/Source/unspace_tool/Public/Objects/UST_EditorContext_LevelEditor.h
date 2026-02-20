// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Editor/UnrealEdTypes.h>
#include <Kismet/KismetSystemLibrary.h>
#include "UST_EditorContext_LevelEditor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventsOnLevelActorAttachedSignature, AActor*, Actor, const AActor*, Parent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventsOnLevelActorDetachedSignature, AActor*, Actor, const AActor*, Parent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElgEditorEventsOnActorMovedSignature, AActor*, Actor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventsOnActorSelectionChangedSignature, const TArray<UObject*>&, NewSelection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventsOnActorSelectedSignature, UObject*, Object);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventsOnActorDeselectedSignature, UObject*, Object);
/**
 * 
 */
UCLASS()
class UNSPACE_TOOL_API UUST_EditorContext_LevelEditor : public UObject
{
	GENERATED_BODY()

public:
#pragma region Setup
	virtual ~UUST_EditorContext_LevelEditor();

	void Setup();
#pragma endregion

#pragma region OnActorAttached
	/* Event when an Actor is added to the level. */
	UPROPERTY(BlueprintAssignable)
	FEventsOnLevelActorAttachedSignature OnLevelActorAttached;

	void HandleOnLevelActorAttached(AActor* InActor, const AActor* InParent);

#pragma endregion

#pragma region OnActorDetached

	/* Event when an Actor is added to the level. */
	UPROPERTY(BlueprintAssignable)
	FEventsOnLevelActorDetachedSignature OnLevelActorDetached;

	void HandleOnLevelActorDetached(AActor* InActor, const AActor* InParent);

#pragma endregion

#pragma region OnActorMoved

	/* Event when an Actor is moved in the level. */
	UPROPERTY(BlueprintAssignable)
	FElgEditorEventsOnActorMovedSignature OnActorMoved;

	void HandleOnActorMoved(AActor* InActor);

#pragma endregion

#pragma region OnActorSelection
	/* Event when the selection in the level editor is changed. */
	UPROPERTY(BlueprintAssignable)
	FEventsOnActorSelectionChangedSignature OnActorSelectionChanged;

	/* Event when a actor is selected. */
	UPROPERTY(BlueprintAssignable)
	FEventsOnActorSelectedSignature OnActorSelected;

	/* Event when a actor is deselected. */
	UPROPERTY(BlueprintAssignable)
	FEventsOnActorDeselectedSignature OnActorDeselected;

	void HandleOnActorSelectionChanged(const TArray<UObject*>& NewSelection, bool bForceRefresh);

	UPROPERTY()
	TArray<UObject*> OldActorSelection;
#pragma endregion	
};

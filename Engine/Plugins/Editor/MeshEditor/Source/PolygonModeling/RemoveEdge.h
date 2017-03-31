// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MeshEditorCommands.h"
#include "RemoveEdge.generated.h"


/** Attempts to remove the selected edge from the polygon, merging the adjacent polygons together */
UCLASS()
class URemoveEdgeCommand : public UMeshEditorEdgeCommand
{
	GENERATED_BODY()

protected:

	// Overrides
	virtual void RegisterUICommand( class FBindingContext* BindingContext ) override;
	virtual void Execute( class IMeshEditorModeEditingContract& MeshEditorMode ) override;
	virtual void AddToVRRadialMenuActionsMenu( class IMeshEditorModeUIContract& MeshEditorMode, class FMenuBuilder& MenuBuilder, TSharedPtr<FUICommandList> CommandList, const FName TEMPHACK_StyleSetName, class UVREditorMode* VRMode ) override;

};

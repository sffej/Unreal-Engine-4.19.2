// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MeshEditorCommands.h"
#include "HardenOrSoftenEdge.generated.h"


/** Makes an edge hard */
UCLASS()
class UHardenEdgeCommand : public UMeshEditorEdgeCommand
{
	GENERATED_BODY()

protected:

	// Overrides
	virtual void RegisterUICommand( class FBindingContext* BindingContext ) override;
	virtual void Execute( class IMeshEditorModeEditingContract& MeshEditorMode ) override;

};


/** Makes an edge soft */
UCLASS()
class USoftenEdgeCommand : public UMeshEditorEdgeCommand
{
	GENERATED_BODY()

protected:

	// Overrides
	virtual void RegisterUICommand( class FBindingContext* BindingContext ) override;
	virtual void Execute( class IMeshEditorModeEditingContract& MeshEditorMode ) override;

};

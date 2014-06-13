// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LevelEditorViewport.h"

class FEdModeLevel : public FEdMode
{
public:

	/** Constructor */
	FEdModeLevel();

	/** Destructor */
	virtual ~FEdModeLevel();

	// Begin FEdMode

	virtual void Exit() override; 

	virtual void AddReferencedObjects( FReferenceCollector& Collector ) override;

	virtual EAxisList::Type GetWidgetAxisToDraw( FWidget::EWidgetMode InWidgetMode ) const override;

	virtual bool ShouldDrawWidget() const override;

	virtual bool UsesTransformWidget(FWidget::EWidgetMode CheckMode) const override;

	virtual FVector GetWidgetLocation() const override;

	virtual bool AllowWidgetMove() override { return true; }

	virtual bool InputDelta( FLevelEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale ) override;

	virtual void Render( const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI ) override;

	virtual bool StartTracking(FLevelEditorViewportClient* InViewportClient, FViewport* InViewport) override;

	virtual bool EndTracking(FLevelEditorViewportClient* InViewportClient, FViewport* InViewport) override;

	virtual bool IsSnapRotationEnabled() override;

	virtual bool SnapRotatorToGridOverride(FRotator& Rotation) override;

	// End FEdMode

	/** Sets the level we will be transforming */ 
	void SetLevel( ULevelStreaming* Level );

	/** Returns true if this is the current level were editing  */ 
	virtual bool IsEditing( ULevelStreaming* Level );

	/** Calls Apply Post Edit Move on all Actors in the level*/
	void ApplyPostEditMove();

private:

	ULevelStreaming* SelectedLevel;
	FTransform LevelTransform;
	UMaterialInstanceDynamic* BoxMaterial;
	FBox LevelBounds;
	bool bIsTracking;
	bool bIsDirty;
};


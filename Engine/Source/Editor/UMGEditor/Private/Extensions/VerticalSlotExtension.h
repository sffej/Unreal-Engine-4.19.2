// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DesignerExtension.h"

class FVerticalSlotExtension : public FDesignerExtension
{
public:
	FVerticalSlotExtension();

	bool IsActive(const TArray< FSelectedWidget >& Selection);
	
	virtual void BuildWidgetsForSelection(const TArray< FSelectedWidget >& Selection, TArray< TSharedRef<SWidget> >& Widgets) override;

private:

	FReply HandleUpPressed();
	FReply HandleDownPressed();

	void MoveUp(UWidget* Widget);
	void MoveDown(UWidget* Widget);

	TArray< FSelectedWidget > SelectionCache;
};
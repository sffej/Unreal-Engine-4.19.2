// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "WidgetTemplate.h"

class FWidgetTemplateButton : public FWidgetTemplate
{
public:
	FWidgetTemplateButton();

	virtual FText GetCategory() override;

	virtual UWidget* Create(UWidgetTree* Tree) override;
};

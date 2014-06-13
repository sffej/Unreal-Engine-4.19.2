// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "K2Node_InputAxisKeyEvent.h"
#include "K2Node_InputVectorAxisEvent.generated.h"

UCLASS(MinimalAPI)
class UK2Node_InputVectorAxisEvent : public UK2Node_InputAxisKeyEvent
{
	GENERATED_UCLASS_BODY()

	// Begin UK2Node interface
	virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	virtual UClass* GetDynamicBindingClass() const override;
	virtual void RegisterDynamicBinding(UDynamicBlueprintBinding* BindingObject) const override;
	// End UK2Node interface
};
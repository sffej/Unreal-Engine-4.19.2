// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#pragma once
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_AIMoveTo.generated.h"

// @TODO this should be moved to AIModule
UCLASS()
class BLUEPRINTGRAPH_API UK2Node_AIMoveTo : public UK2Node_BaseAsyncTask
{
	GENERATED_UCLASS_BODY()

	// Begin UEdGraphNode interface
	virtual FString GetTooltip() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	// End UEdGraphNode interface

	virtual FString GetCategoryName();
};

// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "K2Node.h"
#include "K2Node_CreateDelegate.generated.h"

UCLASS(MinimalAPI)
class UK2Node_CreateDelegate : public UK2Node
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	FName SelectedFunctionName;

	UPROPERTY()
	FGuid SelectedFunctionGuid;

public:
	// UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void PinTypeChanged(UEdGraphPin* Pin) override;
	virtual void NodeConnectionListChanged() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual UObject* GetJumpTargetForDoubleClick() const override;
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual bool IsNodePure() const override { return true; }
	virtual void PostReconstructNode() override;
	virtual class FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& CompilerContext) const override;
	// End of UK2Node interface

	bool IsValid(FString* OutMsg = NULL, bool bDontUseSkeletalClassForSelf = false) const;

	/** Set new Function name (Without notifying about the change) */
	BLUEPRINTGRAPH_API void SetFunction(FName Name);

	BLUEPRINTGRAPH_API UFunction* GetDelegateSignature() const;
	BLUEPRINTGRAPH_API UClass* GetScopeClass(bool bDontUseSkeletalClassForSelf = false) const;

	BLUEPRINTGRAPH_API FName GetFunctionName() const;
	BLUEPRINTGRAPH_API UEdGraphPin* GetDelegateOutPin() const;
	BLUEPRINTGRAPH_API UEdGraphPin* GetObjectInPin() const;
 
	BLUEPRINTGRAPH_API void HandleAnyChange(bool bForceModify = false);
	BLUEPRINTGRAPH_API void HandleAnyChangeWithoutNotifying();

	BLUEPRINTGRAPH_API void ValidationAfterFunctionsAreCreated(class FCompilerResultsLog& MessageLog, bool bFullCompile) const;

	// return Graph and Blueprint, when they should be notified about change. It allows to call BroadcastChanged only once per blueprint.
	BLUEPRINTGRAPH_API void HandleAnyChange(UEdGraph* & OutGraph, UBlueprint* & OutBlueprint);
};

// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "BehaviorTreeEditorPrivatePCH.h"
#include "DetailCustomizations/BehaviorDecoratorDetails.h"
#include "DetailCustomizations/BlackboardDecoratorDetails.h"
#include "DetailCustomizations/BlackboardSelectorDetails.h"

#include "ModuleManager.h"
#include "Toolkits/ToolkitManager.h"
#include "SGraphNode_BehaviorTree.h"
#include "SGraphNode_Decorator.h"
#include "EdGraphUtilities.h"


IMPLEMENT_MODULE( FBehaviorTreeEditorModule, BehaviorTreeEditor );
DEFINE_LOG_CATEGORY(LogBehaviorTreeEditor);

const FName FBehaviorTreeEditorModule::BehaviorTreeEditorAppIdentifier( TEXT( "BehaviorTreeEditorApp" ) );

class FGraphPanelNodeFactory_BehaviorTree : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UBehaviorTreeGraphNode* BTNode = Cast<UBehaviorTreeGraphNode>(Node))
		{
			return SNew(SGraphNode_BehaviorTree, BTNode);
		}

		if (UBehaviorTreeDecoratorGraphNode_Decorator* InnerNode = Cast<UBehaviorTreeDecoratorGraphNode_Decorator>(Node))
		{
			return SNew(SGraphNode_Decorator, InnerNode);
		}

		return NULL;
	}
};

TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_BehaviorTree;

void FBehaviorTreeEditorModule::StartupModule()
{
	MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

	GraphPanelNodeFactory_BehaviorTree = MakeShareable( new FGraphPanelNodeFactory_BehaviorTree() );
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_BehaviorTree);

	IAssetTools& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	TSharedPtr<FAssetTypeActions_BehaviorTree> BehaviorTreeAssetTypeAction = MakeShareable(new FAssetTypeActions_BehaviorTree);
	ItemDataAssetTypeActions.Add(BehaviorTreeAssetTypeAction);
	AssetToolsModule.RegisterAssetTypeActions(BehaviorTreeAssetTypeAction.ToSharedRef());

	TSharedPtr<FAssetTypeActions_Blackboard> BlackboardAssetTypeAction = MakeShareable(new FAssetTypeActions_Blackboard);
	ItemDataAssetTypeActions.Add(BlackboardAssetTypeAction);
	AssetToolsModule.RegisterAssetTypeActions(BlackboardAssetTypeAction.ToSharedRef());

	// Register the details customizer
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout( "BlackboardKeySelector", FOnGetPropertyTypeCustomizationInstance::CreateStatic( &FBlackboardSelectorDetails::MakeInstance ) );
	PropertyModule.RegisterCustomClassLayout( "BTDecorator_Blackboard", FOnGetDetailCustomizationInstance::CreateStatic( &FBlackboardDecoratorDetails::MakeInstance ) );
	PropertyModule.RegisterCustomClassLayout( "BTDecorator", FOnGetDetailCustomizationInstance::CreateStatic( &FBehaviorDecoratorDetails::MakeInstance ) );
	PropertyModule.NotifyCustomizationModuleChanged();
}


void FBehaviorTreeEditorModule::ShutdownModule()
{
	if (!UObjectInitialized())
	{
		return;
	}

	MenuExtensibilityManager.Reset();
	ToolBarExtensibilityManager.Reset();
	ClassCache.Reset();

	if ( GraphPanelNodeFactory_BehaviorTree.IsValid() )
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_BehaviorTree);
		GraphPanelNodeFactory_BehaviorTree.Reset();
	}

	// Unregister the BehaviorTree item data asset type actions
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for(auto& AssetTypeAction : ItemDataAssetTypeActions)
		{
			if (AssetTypeAction.IsValid())
			{
				AssetToolsModule.UnregisterAssetTypeActions(AssetTypeAction.ToSharedRef());
			}	
		}			
	}
	ItemDataAssetTypeActions.Empty();

	// Unregister the details customization
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout( "BlackboardKeySelector" );
		PropertyModule.UnregisterCustomClassLayout( "BTDecorator_Blackboard" );
		PropertyModule.UnregisterCustomClassLayout( "BTDecorator" );
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}

void FBehaviorTreeEditorModule::HandleExperimentalSettingChanged(FName PropertyName)
{
	if (PropertyName == TEXT("bBehaviorTreeEditor"))
	{
		UBehaviorTreeFactory* EditorUtilityBlueprintFactory = Cast<UBehaviorTreeFactory>(UBehaviorTreeFactory::StaticClass()->GetDefaultObject());
		EditorUtilityBlueprintFactory->bCreateNew = GetDefault<UEditorExperimentalSettings>()->bBehaviorTreeEditor;;
	}
}

TSharedRef<IBehaviorTreeEditor> FBehaviorTreeEditorModule::CreateBehaviorTreeEditor( const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UObject* Object )
{
	if (!ClassCache.IsValid())
	{
		ClassCache = MakeShareable(new FClassBrowseHelper);
	}

	TSharedRef< FBehaviorTreeEditor > NewBehaviorTreeEditor( new FBehaviorTreeEditor() );
	NewBehaviorTreeEditor->InitBehaviorTreeEditor( Mode, InitToolkitHost, Object );
	return NewBehaviorTreeEditor;	
}

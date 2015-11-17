// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MovieSceneToolsPrivatePCH.h"
#include "ModuleManager.h"
#include "ISequencerModule.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "MovieSceneSection.h"
#include "ISequencerSection.h"
#include "ScopedTransaction.h"
#include "MovieScene.h"
#include "MovieSceneTrackEditor.h"

#include "BoolPropertyTrackEditor.h"
#include "BytePropertyTrackEditor.h"
#include "ColorPropertyTrackEditor.h"
#include "FloatPropertyTrackEditor.h"
#include "VectorPropertyTrackEditor.h"
#include "VisibilityPropertyTrackEditor.h"

#include "TransformTrackEditor.h"
#include "ShotTrackEditor.h"
#include "SlomoTrackEditor.h"
#include "SubTrackEditor.h"
#include "AudioTrackEditor.h"
#include "SkeletalAnimationTrackEditor.h"
#include "ParticleTrackEditor.h"
#include "ParticleParameterTrackEditor.h"
#include "AttachTrackEditor.h"
#include "EventTrackEditor.h"
#include "PathTrackEditor.h"
#include "MaterialTrackEditor.h"
#include "FadeTrackEditor.h"
#include "SpawnTrackEditor.h"

#include "MovieSceneClipboard.h"
#include "SequencerClipboardReconciler.h"
#include "ClipboardTypes.h"
#include "Curves/CurveBase.h"


/**
 * Implements the MovieSceneTools module.
 */
class FMovieSceneToolsModule
	: public IMovieSceneTools
{
public:

	// IModuleInterface interface

	virtual void StartupModule() override
	{
		ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>( "Sequencer" );

		// register property track editors
		BoolPropertyTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FBoolPropertyTrackEditor::CreateTrackEditor ) );
		BytePropertyTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FBytePropertyTrackEditor::CreateTrackEditor ) );
		ColorPropertyTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FColorPropertyTrackEditor::CreateTrackEditor ) );
		FloatPropertyTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FFloatPropertyTrackEditor::CreateTrackEditor ) );
		VectorPropertyTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FVectorPropertyTrackEditor::CreateTrackEditor ) );
		VisibilityPropertyTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FVisibilityPropertyTrackEditor::CreateTrackEditor ) );

		// register specialty track editors
		AnimationTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FSkeletalAnimationTrackEditor::CreateTrackEditor ) );
		AttachTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &F3DAttachTrackEditor::CreateTrackEditor ) );
		AudioTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FAudioTrackEditor::CreateTrackEditor ) );
		EventTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FEventTrackEditor::CreateTrackEditor ) );
		ParticleTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FParticleTrackEditor::CreateTrackEditor ) );
		ParticleParameterTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FParticleParameterTrackEditor::CreateTrackEditor ) );
		PathTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &F3DPathTrackEditor::CreateTrackEditor ) );
		ShotTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FShotTrackEditor::CreateTrackEditor ) );
		SlomoTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FSlomoTrackEditor::CreateTrackEditor ) );
		SubTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FSubTrackEditor::CreateTrackEditor ) );
		TransformTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &F3DTransformTrackEditor::CreateTrackEditor ) );
		ComponentMaterialTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FComponentMaterialTrackEditor::CreateTrackEditor ) );
		FadeTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FFadeTrackEditor::CreateTrackEditor ) );
		SpawnTrackCreateEditorHandle = SequencerModule.RegisterTrackEditor_Handle( FOnCreateTrackEditor::CreateStatic( &FSpawnTrackEditor::CreateTrackEditor ) );

		RegisterClipboardConversions();
	}

	virtual void ShutdownModule() override
	{
		if (!FModuleManager::Get().IsModuleLoaded("Sequencer"))
		{
			return;
		}

		ISequencerModule& SequencerModule = FModuleManager::Get().GetModuleChecked<ISequencerModule>( "Sequencer" );

		// unregister property track editors
		SequencerModule.UnRegisterTrackEditor_Handle( BoolPropertyTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( BytePropertyTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( ColorPropertyTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( FloatPropertyTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( VectorPropertyTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( VisibilityPropertyTrackCreateEditorHandle );

		// unregister specialty track editors
		SequencerModule.UnRegisterTrackEditor_Handle( AnimationTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( AttachTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( AudioTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( EventTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( ParticleTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( ParticleParameterTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( PathTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( ShotTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( SlomoTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( SubTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( TransformTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( ComponentMaterialTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( FadeTrackCreateEditorHandle );
		SequencerModule.UnRegisterTrackEditor_Handle( SpawnTrackCreateEditorHandle );
	}

	void RegisterClipboardConversions()
	{
		using namespace MovieSceneClipboard;

		DefineImplicitConversion<int32, uint8>();
		DefineImplicitConversion<int32, bool>();

		DefineImplicitConversion<uint8, int32>();
		DefineImplicitConversion<uint8, bool>();

		DefineExplicitConversion<int32, FRichCurveKey>([](const int32& In) -> FRichCurveKey { return FRichCurveKey(0.f, In);	});
		DefineExplicitConversion<uint8, FRichCurveKey>([](const uint8& In) -> FRichCurveKey { return FRichCurveKey(0.f, In);	});
		DefineExplicitConversion<FRichCurveKey, int32>([](const FRichCurveKey& In) -> int32 { return In.Value; 					});
		DefineExplicitConversion<FRichCurveKey, uint8>([](const FRichCurveKey& In) -> uint8 { return In.Value; 					});
		DefineExplicitConversion<FRichCurveKey, bool>([](const FRichCurveKey& In) -> bool	{ return !!In.Value; 				});

		FSequencerClipboardReconciler::AddTrackAlias("Location.X", "R");
		FSequencerClipboardReconciler::AddTrackAlias("Location.Y", "G");
		FSequencerClipboardReconciler::AddTrackAlias("Location.Z", "B");

		FSequencerClipboardReconciler::AddTrackAlias("Rotation.X", "R");
		FSequencerClipboardReconciler::AddTrackAlias("Rotation.Y", "G");
		FSequencerClipboardReconciler::AddTrackAlias("Rotation.Z", "B");

		FSequencerClipboardReconciler::AddTrackAlias("Scale.X", "R");
		FSequencerClipboardReconciler::AddTrackAlias("Scale.Y", "G");
		FSequencerClipboardReconciler::AddTrackAlias("Scale.Z", "B");

		FSequencerClipboardReconciler::AddTrackAlias("X", "R");
		FSequencerClipboardReconciler::AddTrackAlias("Y", "G");
		FSequencerClipboardReconciler::AddTrackAlias("Z", "B");
		FSequencerClipboardReconciler::AddTrackAlias("W", "A");
	}

private:

	/** Registered delegate handles */
	FDelegateHandle BoolPropertyTrackCreateEditorHandle;
	FDelegateHandle BytePropertyTrackCreateEditorHandle;
	FDelegateHandle ColorPropertyTrackCreateEditorHandle;
	FDelegateHandle FloatPropertyTrackCreateEditorHandle;
	FDelegateHandle VectorPropertyTrackCreateEditorHandle;
	FDelegateHandle VisibilityPropertyTrackCreateEditorHandle;

	FDelegateHandle AnimationTrackCreateEditorHandle;
	FDelegateHandle AttachTrackCreateEditorHandle;
	FDelegateHandle AudioTrackCreateEditorHandle;
	FDelegateHandle EventTrackCreateEditorHandle;
	FDelegateHandle ParticleTrackCreateEditorHandle;
	FDelegateHandle ParticleParameterTrackCreateEditorHandle;
	FDelegateHandle PathTrackCreateEditorHandle;
	FDelegateHandle ShotTrackCreateEditorHandle;
	FDelegateHandle SlomoTrackCreateEditorHandle;
	FDelegateHandle SubTrackCreateEditorHandle;
	FDelegateHandle TransformTrackCreateEditorHandle;
	FDelegateHandle ComponentMaterialTrackCreateEditorHandle;
	FDelegateHandle FadeTrackCreateEditorHandle;
	FDelegateHandle SpawnTrackCreateEditorHandle;
};


IMPLEMENT_MODULE( FMovieSceneToolsModule, MovieSceneTools );

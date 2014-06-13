// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

/**
 * Instance of a UMovieSceneTransformTrack
 */
class FMovieSceneTransformTrackInstance : public IMovieSceneTrackInstance
{
public:
	FMovieSceneTransformTrackInstance( UMovieSceneTransformTrack& InTransformTrack );

	/** IMovieSceneTrackInstance interface */
	virtual void Update( float Position, float LastPosition, const TArray<UObject*>& RuntimeObjects, class IMovieScenePlayer& Player ) override;
	virtual void RefreshInstance( IMovieScenePlayer& Player ) override {}
private:
	/** Track that is being instanced */
	UMovieSceneTransformTrack* TransformTrack;
};

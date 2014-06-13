// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

/**
 * Instance of a UMovieSceneFloatTrack
 */
class FMovieSceneFloatTrackInstance : public IMovieSceneTrackInstance
{
public:
	FMovieSceneFloatTrackInstance( UMovieSceneFloatTrack& InFloatTrack );

	/** IMovieSceneTrackInstance interface */
	virtual void Update( float Position, float LastPosition, const TArray<UObject*>& RuntimeObjects, class IMovieScenePlayer& Player ) override;
	virtual void RefreshInstance( IMovieScenePlayer& Player ) override {}
private:
	/** Track that is being instanced */
	UMovieSceneFloatTrack* FloatTrack;
};

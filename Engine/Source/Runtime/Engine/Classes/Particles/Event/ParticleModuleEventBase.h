// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#pragma once
#include "Particles/ParticleModule.h"
#include "ParticleModuleEventBase.generated.h"

UCLASS(editinlinenew, hidecategories=Object, abstract, meta=(DisplayName = "Event"))
class UParticleModuleEventBase : public UParticleModule
{
	GENERATED_UCLASS_BODY()


	// Begin UParticleModule Interface
	virtual EModuleType	GetModuleType() const override {	return EPMT_Event;	}
	// End UParticleModule Interface
};


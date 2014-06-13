// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SCurveEditor.h"

class FWheeledVehicleMovementComponent4WDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails( IDetailLayoutBuilder& DetailBuilder ) override;
	
private:

	struct FSteeringCurveEditor : public FCurveOwnerInterface
	{
		FSteeringCurveEditor(UWheeledVehicleMovementComponent4W * InVehicle = NULL);
		/** FCurveOwnerInterface interface */
		virtual TArray<FRichCurveEditInfoConst> GetCurves() const override;
		virtual TArray<FRichCurveEditInfo> GetCurves() override;
		virtual UObject* GetOwner() override;
		virtual void ModifyOwner() override;
		virtual void MakeTransactional() override;

	private:
		UWheeledVehicleMovementComponent4W * VehicleComponent;
		UObject * Owner;

	} SteeringCurveEditor;

	struct FTorqueCurveEditor : public FCurveOwnerInterface
	{
		FTorqueCurveEditor(UWheeledVehicleMovementComponent4W * InVehicle = NULL);
		/** FCurveOwnerInterface interface */
		virtual TArray<FRichCurveEditInfoConst> GetCurves() const override;
		virtual TArray<FRichCurveEditInfo> GetCurves() override;
		virtual UObject* GetOwner() override;
		virtual void ModifyOwner() override;
		virtual void MakeTransactional() override;

	private:
		UWheeledVehicleMovementComponent4W * VehicleComponent;
		UObject * Owner;

	} TorqueCurveEditor;

	TArray<TWeakObjectPtr<UObject> > SelectedObjects;	//the objects we're showing details for
	
	/** Steering curve widget */
	TSharedPtr<class SCurveEditor> SteeringCurveWidget;

	/** Torque curve widget */
	TSharedPtr<class SCurveEditor> TorqueCurveWidget;
};


// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	IOSTargetDevice.h: Defines an IOS device for communication
=============================================================================*/

#pragma once


/**
 * Type definition for shared pointers to instances of FIOSTargetDevice.
 */
typedef TSharedPtr<class FIOSTargetDevice, ESPMode::ThreadSafe> FIOSTargetDevicePtr;

/**
 * Type definition for shared references to instances of FIOSTargetDevice.
 */
typedef TSharedRef<class FIOSTargetDevice, ESPMode::ThreadSafe> FIOSTargetDeviceRef;


/**
 * Implements an iOS target device.
 */
class FIOSTargetDevice : public ITargetDevice
{
public:
	FIOSTargetDevice (const ITargetPlatform& InTargetPlatform)
		: TargetPlatform(InTargetPlatform)
		, bCanReboot(false)
		, bCanPowerOn(false)
		, bCanPowerOff(false)
		, DeviceType(ETargetDeviceTypes::Indeterminate)
		, DeviceEndpoint()
		, AppId()
	{
		DeviceId = FTargetDeviceId(TargetPlatform.PlatformName(), FPlatformProcess::ComputerName());
		DeviceName = FPlatformProcess::ComputerName();
		MessageEndpoint = FMessageEndpoint::Builder("FIOSTargetDevice").Build();
	}
public:
	// ITargetDevice interface
	virtual bool Connect() override;
	virtual bool Deploy(const FString& SourceFolder, FString& OutAppId) override;
	virtual void Disconnect() override;
	virtual int32 GetProcessSnapshot(TArray<FTargetDeviceProcessInfo>& OutProcessInfos) override;
	virtual ETargetDeviceTypes::Type GetDeviceType() const override;
	virtual FTargetDeviceId GetId() const override;
	virtual FString GetName() const override;
	virtual FString GetOperatingSystemName() override;
	virtual const class ITargetPlatform& GetTargetPlatform() const override;
	virtual bool IsConnected() override;
	virtual bool IsDefault() const override;
	virtual bool Launch(const FString& InAppId, EBuildConfigurations::Type InBuildConfiguration, EBuildTargets::Type BuildTarget, const FString& Params, uint32* OutProcessId) override;
	virtual bool PowerOff(bool Force) override;
	virtual bool PowerOn() override;
	virtual bool Reboot(bool bReconnect = false) override;
	virtual bool Run(const FString& ExecutablePath, const FString& Params, uint32* OutProcessId) override;
	virtual bool SupportsFeature(ETargetDeviceFeatures::Type Feature) const;
	virtual bool SupportsSdkVersion(const FString& VersionString) const override;
	virtual bool TerminateProcess(const int32 ProcessId) override;
	virtual void SetUserCredentials(const FString & UserName, const FString & UserPassword) override;
	virtual bool GetUserCredentials(FString & OutUserName, FString & OutUserPassword) override;
	// End of ITargetDevice interface

public:
	/** Timeout check for removing stale devices */
	FDateTime LastPinged;

private:

	/** The current status of this device. */
//	ETargetDeviceStatus::Type Status;
	/** Holds a reference to the device's target platform. */
	const ITargetPlatform& TargetPlatform;
	/** Contains the address of the remote device */
	FMessageAddress DeviceEndpoint;
	/** MessageEndpoint for communicating with remote device */
	FMessageEndpointPtr MessageEndpoint;
	/** Contains the current AppID/GameName for Deployment/launching. */
	FString AppId;
	/** Containts the build configuration of the app to deploy */
	EBuildConfigurations::Type BuildConfiguration;
	/** Lets us know whether the thing is a sim device or a physical device. */
	bool bIsSimulated;


private:

	/** Remote rebootable */
	bool bCanReboot;
	/** Remote bootable */
	bool bCanPowerOn;
	/** Remote shutdown-able */
	bool bCanPowerOff;
	/** Id of device */
	FTargetDeviceId DeviceId;
	/** Name of device */
	FString DeviceName;
	/** Type of device */
	ETargetDeviceTypes::Type DeviceType;

public:

	void SetFeature(ETargetDeviceFeatures::Type InFeature, bool bFlag)
	{
		if(InFeature == ETargetDeviceFeatures::Reboot)
		{
			bCanReboot = bFlag;
		}
		else if(InFeature == ETargetDeviceFeatures::PowerOn)
		{
			bCanPowerOn = bFlag;
		}
		else if(InFeature == ETargetDeviceFeatures::PowerOff)
		{
			bCanPowerOff = bFlag;
		}
	}

	/** Sets device id */
	void SetDeviceId(const FTargetDeviceId InDeviceId)
	{
		DeviceId = InDeviceId;
	}

	/** Sets the name of the device */
	void SetDeviceName(const FString InDeviceName)
	{
		DeviceName = InDeviceName;
	}

	/** Sets the type of the device */
	void SetDeviceType(const FString InDeviceTypeString)
	{
		if(InDeviceTypeString == TEXT("Browser"))
		{
			DeviceType = ETargetDeviceTypes::Browser;
		}
		else if(InDeviceTypeString == TEXT("Console"))
		{
			DeviceType = ETargetDeviceTypes::Console;
		}
		else if(InDeviceTypeString == TEXT("Phone"))
		{
			DeviceType = ETargetDeviceTypes::Phone;
		}
		else if(InDeviceTypeString == TEXT("Tablet"))
		{
			DeviceType = ETargetDeviceTypes::Tablet;
		}
		else
		{
			DeviceType = ETargetDeviceTypes::Indeterminate;
		}
	}

	void SetDeviceEndpoint(const FMessageAddress& DeviceAddress)
	{
		DeviceEndpoint = DeviceAddress;
	}

	void SetAppId(const FString& GameName)
	{
		AppId = GameName;
	}

	void SetAppConfiguration(EBuildConfigurations::Type Configuration)
	{
		BuildConfiguration = Configuration;
	}

	void SetIsSimulated(bool IsSimulated)
	{
		bIsSimulated = IsSimulated;
	}
};

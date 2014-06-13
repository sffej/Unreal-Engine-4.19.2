// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

//
// Basic beacon
//
#pragma once
#include "Runtime/Online/OnlineSubsystemUtils/Classes/OnlineBeacon.h"
#include "OnlineBeaconHost.generated.h"

class AOnlineBeaconHostObject;
class AOnlineBeaconClient;

/**
 * Base class for any actor that would like to communicate outside the normal Unreal Networking gameplay path
 *
 * The host beacon listens for connections and is responsible for spawning the actual actor for communication host side
 * This actor was initiated by the client, the host verifies the validity and accepts/continues the connection
 */
UCLASS(transient, notplaceable, config=Engine)
class ONLINESUBSYSTEMUTILS_API AOnlineBeaconHost : public AOnlineBeacon
{
	GENERATED_UCLASS_BODY()

	/** Configured listen port for this beacon host */
	UPROPERTY(Config)
	int32 ListenPort;

	// Begin AActor Interface
	virtual void OnNetCleanup(class UNetConnection* Connection) override;
	// End AActor Interface

	// Begin OnlineBeacon Interface
	virtual void HandleNetworkFailure(UWorld* World, class UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString) override;
	// End OnlineBeacon Interface

	// Begin FNetworkNotify Interface
	virtual void NotifyControlMessage(class UNetConnection* Connection, uint8 MessageType, class FInBunch& Bunch) override;
	// End FNetworkNotify Interface

	/**
	 * Initialize the host beacon on a specified port
	 *	Creates the net driver and begins listening for connections
	 *
	 * @return true if host was setup correctly, false otherwise
	 */
	virtual bool InitHost();

	/**
	 * Get the listen port for this beacon
	 *
	 * @return beacon listen port
	 */
	virtual int32 GetListenPort() { return ListenPort; }

	/**
	 * Register a beacon host and its client actor factory
	 *
	 * @param NewHostObject new 
	 */
	virtual void RegisterHost(AOnlineBeaconHostObject* NewHostObject);

	/**
	 * Unregister a beacon host, making future connections of a given type unresponsive
	 *
	 * @param BeaconType type of beacon host to unregister
	 */
	virtual void UnregisterHost(const FString& BeaconType);

	/**
	 * Get the host responsible for a given beacon type
	 *
	 * @param BeaconType type of beacon host 
	 * 
	 * @return BeaconHost for the given type or NULL if that type is not registered
	 */
	AOnlineBeaconHostObject* GetHost(const FString& BeaconType);

	/**
	 * Save actors on seamless travel
	 * see AGameMode::GetSeamlessTravelActorList
	 */
	void GetSeamlessTravelActorList(bool bToEntry, TArray<AActor*>& ActorList);

	/**
	 * Get a client beacon actor for a given connection
	 *
	 * @param Connection connection of interest
	 *
	 * @return client beacon actor that owns the connection
	 */
	virtual AOnlineBeaconClient* GetClientActor(class UNetConnection* Connection);

	/**
	 * Remove a client beacon actor from the list of active connections
	 *
	 * @param ClientActor client beacon actor to remove
	 */
	virtual void RemoveClientActor(AOnlineBeaconClient* ClientActor);

private:

	/** List of all client beacon actors with active connections */
	UPROPERTY()
	TArray<AOnlineBeaconClient*> ClientActors;

	/** Delegate to route a connection attempt to the appropriate beacon host, by type */
	DECLARE_DELEGATE_RetVal_OneParam(AOnlineBeaconClient*, FOnBeaconSpawned, class UNetConnection*);
	FOnBeaconSpawned& OnBeaconSpawned(const FString& BeaconType);

	/** Mapping of beacon types to the OnBeaconSpawned delegates */
	TMap<FString, FOnBeaconSpawned> OnBeaconSpawnedMapping;

	/** Delegate to route a connection event to the appropriate beacon host, by type */
	DECLARE_DELEGATE_TwoParams(FOnBeaconConnected, AOnlineBeaconClient*, class UNetConnection*);
	FOnBeaconConnected& OnBeaconConnected(const FString& BeaconType);

	/** Mapping of beacon types to the OnBeaconConnected delegates */
	TMap<FString, FOnBeaconConnected> OnBeaconConnectedMapping;
};
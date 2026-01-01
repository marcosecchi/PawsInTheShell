// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_WorldSubsystem.h"
#include "PITS_ObjectPoolSubsystem.generated.h"

class UPITS_FixedActorPool;

/**
 * World subsystem that manages fixed-size pools of actor instances using UObject-based
 * pool objects (UPITS_FixedActorPool). Each pool is stored in `ActorsPoolMap` and owned
 * by the Unreal object system via `TObjectPtr`.
 *
 * Implementation notes:
 * - `ActorsPoolMap` is marked with `UPROPERTY()` so the map and its UObject references
 *   are visible to the Garbage Collector.
 * - Pools are UObject-derived (`UPITS_FixedActorPool`) so they can be returned from
 *   `UFUNCTION`s and used in Blueprint.
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_ObjectPoolSubsystem : public UPITS_WorldSubsystem
{
	GENERATED_BODY()
	
	/** Map of actor classes to their corresponding object pools. */
	UPROPERTY()
	TMap<TSubclassOf<AActor>, TObjectPtr<class UPITS_FixedActorPool>> ActorsPoolMap;

protected:
	/** Retrieves the object pool for the specified actor class. */
	UFUNCTION(Category="PawsInTheShell|Subsystems")
	UPITS_FixedActorPool* GetObjectPool(const TSubclassOf<AActor> SpawnableClass) const;

public:
	
	/** Creates an object pool for the specified actor class with the given pool size. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void CreateObjectPool(TSubclassOf<AActor> SpawnableClass, int32 PoolSize = 20);
	
	/** Checks if an object pool exists for the specified actor class. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool HasObjectPool(TSubclassOf<AActor> SpawnableClass) const;
	
	/** Checks if there are available pooled objects for the specified actor class. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool HasAvailablePooledObjects(const TSubclassOf<AActor> SpawnableClass) const;

	/** Checks if the specified actor is managed by any object pool. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool IsObjectPooled(const AActor* Actor) const;
	
	/** Acquires a pooled object of the specified class and sets its transform. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	AActor* AcquirePooledObject(const TSubclassOf<AActor> SpawnableClass, const FTransform ObjectTransform);
	
	/** Releases the specified actor back to its object pool. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void ReleasePooledObject(AActor* Actor);
};

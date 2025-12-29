// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PITS_FixedActorPool.generated.h"

/**
 * This UObject-based pool spawns and manages a fixed number of actors in the provided UWorld.
 * Use InitializePool() to supply the actor class and desired size. Acquire actors with
 * GetObjectFromPool() and return them with ReleaseObjectToPool().
 *
 * Notes:
 * - The pool participates in UE reflection and garbage collection.
 * - Not thread-safe.
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_FixedActorPool : public UObject
{
	GENERATED_BODY()

	/* The pool of actors */
	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> ActorPool;

	/* The class of actors to spawn and pool */
	TSubclassOf<AActor> TheSpawnableClass;

	/* Size of the pool */
	UPROPERTY()
	int32 ThePoolSize = 10;

public:
	/* Initialize with the actor class to spawn and desired pool size. */
	void InitializePool(TSubclassOf<AActor> SpawnableClass, int32 PoolSize);
	
	/*  Acquire an actor from the pool. Returns nullptr if none are available. */
	AActor* GetObjectFromPool();
	
	/* Release an actor back to the pool. */
	void ReleaseObjectToPool(AActor* Actor);

	/* Check whether there are available actors in the pool. */
	bool HasAvailableObjectsInPool() const;
	
	/* Check whether the given actor belongs to this pool. */
	bool IsObjectPooled(const AActor* Actor) const;
};

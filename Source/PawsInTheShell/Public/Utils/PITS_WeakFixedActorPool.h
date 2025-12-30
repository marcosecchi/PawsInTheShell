// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"

/**
 * This lightweight C++-only pool spawns and manages a fixed number of actors in the provided
 * UWorld, keeping only TWeakObjectPtr\<AActor\> references so actor lifetimes are not extended.
 * Use InitializePool() to supply the UWorld, actor class and desired size. Acquire actors with
 * GetObjectFromPool() and return them with ReleaseObjectToPool().
 *
 * Notes:
 * - The pool does not participate in UE reflection or garbage collection; it relies on weak
 *   pointers to detect destroyed actors.
 * - Caller must ensure the provided UWorld remains valid while the pool is used.
 * - Not thread-safe.
 */
class FPITS_WeakFixedActorPool
{
	/* The world context for spawning actors. */
	UWorld* TheWorld = nullptr;
	
	/* The class of actors to spawn and pool. */
	TSubclassOf<AActor> TheSpawnableClass;
	
	/* The pool of weak actor pointers. */
	TArray<TWeakObjectPtr<AActor>> TheActorPool;
	
	/* Stack of free indices for quick allocation. */
	TArray<int32> FreeIndices;
	
	/* Map from actor to its index in the pool for quick lookup. */
	TMap<AActor*, int32> ActorToIndex;
	
public:
	FPITS_WeakFixedActorPool() = default;
	~FPITS_WeakFixedActorPool() = default;

	/* Initialize the pool with the given world, actor class, and pool size. */
	void InitializePool(UWorld* InWorld, const TSubclassOf<AActor> InSpawnableClass, const int32 InPoolSize = 10);

	/* Acquire an actor from the pool. Returns nullptr if none are available. */
	AActor* GetObjectFromPool();
	
	/* Release an actor back to the pool. */
	void ReleaseObjectToPool(AActor* Actor);

	/* Check whether there are available actors in the pool. */
	bool HasAvailableObjectsInPool() const;
	
	/* Check whether the given actor belongs to this pool. */
	bool IsObjectPooled(const AActor* Actor) const;
	
};

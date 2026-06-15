// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LazyActorPool.generated.h"

/**
 * 
 */
UCLASS()
class PAWSINTHESHELL_API ULazyActorPool : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsPoolInitialized = false;
	
	/* The pool of actors */
	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> TheActorPool;

	UPROPERTY()
	TSubclassOf<AActor> TheSpawnableClass = nullptr;

	/* Add a new object to the pool */
	AActor* AddObjectToPool();
	
	/* Add a set of objects to the pool */
	void AddObjectsToPool(int32 PoolSize);

public:
	/* Initialize with the actor class to spawn and desired pool size. */
	void InitializePool(TSubclassOf<AActor> SpawnableClass, const int32 PoolSize = 0);
	
	/*  Acquire an actor from the pool. Returns nullptr if none are available. */
	AActor* RequestObjectFromPool();
	
	/* Release an actor back to the pool. */
	void ReleaseObjectToPool(AActor* Actor);

	/* Check whether there are available actors in the pool. */
	bool HasAvailableObjectsInPool() const;
	
	/* Check whether the given actor belongs to this pool. */
	bool IsObjectPooled(const AActor* Actor) const;

#if WITH_DEV_AUTOMATION_TESTS
public:
	/* Get the number of actors currently in the pool. */
	int32 GetPoolSize() const { return TheActorPool.Num(); }
	
	void Empty()
	{
		for (AActor* Actor : TheActorPool)
		{
			Actor->Destroy();
		}
		TheActorPool.Empty();
	}
#endif
};

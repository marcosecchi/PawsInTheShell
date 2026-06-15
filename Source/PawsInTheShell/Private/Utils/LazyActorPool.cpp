// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Utils/LazyActorPool.h"

#include "Utils/PITS_Globals.h"
#include "Utils/PITS_Logs.h"

void ULazyActorPool::InitializePool(const TSubclassOf<AActor> SpawnableClass, const int32 PoolSize)
{
	if (bIsPoolInitialized)
	{
		UE_LOG(LogPITS, Log, TEXT("Pool %s is already initialized. Skipping re-initialization."), *GetName());
		return;
	}
	
	TheSpawnableClass = SpawnableClass;
	
	UE_LOG(LogPITS, Log, TEXT("Spawning %d actors of %s for pool initialization"), PoolSize, *GetName());
	// Pre-spawn the specified number of actors and add them to the pool.
	for (int32 i = 0; i < PoolSize; ++i)
	{
		AddObjectToPool();
	}
}

AActor* ULazyActorPool::RequestObjectFromPool()
{
	// Iterate through the pool to find an inactive (hidden) actor.
	for (TObjectPtr<AActor>& ActorPtr : TheActorPool)
	{
		// Guard against null pointers.
		if (ActorPtr)
		{
			// Check whether the actor is currently 'inactive' (hidden) and can be reused.
			if (AActor* Actor = ActorPtr.Get(); Actor->IsHidden())
			{
				Actor->SetActorHiddenInGame(false);
				Actor->SetActorEnableCollision(true);
				
				UE_LOG(LogPITS, Log, TEXT("Acquired actor %s from pool %s"), *Actor->GetName(), *GetName());
				return Actor;
			}
		}
	}

	UE_LOG(LogPITS, Log, TEXT("No available actors in pool %s. Adding a new actor to the pool."), *GetName());
	AddObjectToPool();
	return nullptr;
}

void ULazyActorPool::ReleaseObjectToPool(AActor* Actor)
{
	// Macro to guard against null pointers and log a message if null.
	CHECK_PTR_AND_LOG_RETURN(Actor);

	// Find the actor in the pool.
	for (TObjectPtr<AActor>& ActorPtr : TheActorPool)
	{
		// Check if this is the actor being released.
		if (ActorPtr && ActorPtr.Get() == Actor)
		{
			// Mark the actor as inactive so it can be reused later.
			Actor->SetActorHiddenInGame(true);
			Actor->SetActorEnableCollision(false);
			
			UE_LOG(LogPITS, Log, TEXT("Released actor %s back to pool in %s"), *Actor->GetName(), *GetName());
			return;
		}
	}

	// If we reach here the actor was not found in this pool.
	UE_LOG(LogPITS, Warning, TEXT("Attempted to release an actor not from this pool %s"), *GetName());
}

bool ULazyActorPool::HasAvailableObjectsInPool() const
{
	// Check if there is at least one actor in the pool that is currently 'inactive' (hidden).
	return TheActorPool.ContainsByPredicate([](const TObjectPtr<AActor>& ActorPtr)
	{
		return ActorPtr && ActorPtr->IsHidden();
	});
}

bool ULazyActorPool::IsObjectPooled(const AActor* Actor) const
{
	// Macro to guard against null pointers and log a message if null.
	if (Actor == nullptr) 
	{
		UE_LOG(LogPITS, Warning, TEXT("IsObjectPooled called with null Actor in pool %s"), *GetName());
		return false;
	}
	// Check if the actor exists in the pool and return the result.
	return (TheActorPool.Contains(Actor));
}

AActor* ULazyActorPool::AddObjectToPool()
{
	// Get the world context from the UObject.
	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		return nullptr;
	}

	// Always spawn even if collisions might overlap during initialization.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn the actor and, if successful, deactivate it for pooling.
	AActor* NewActor = World->SpawnActor<AActor>(TheSpawnableClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (!IsValid(NewActor))
	{
		// Log spawn failures for debugging spawn class / world issues.
		UE_LOG(LogPITS, Warning, TEXT("Failed to spawn actor for pool in %s"), *GetName());
		return nullptr;
	}
	// Ensure the actor is inactive until explicitly acquired.
	NewActor->SetActorEnableCollision(false);
	NewActor->SetActorHiddenInGame(true);
	// Prevent automatic destruction by zeroing lifespan.
	NewActor->SetLifeSpan(0.f);
	TheActorPool.Add(NewActor);
	return NewActor;
}

void ULazyActorPool::AddObjectsToPool(const int32 PoolSize)
{
	for (int32 i = 0; i < PoolSize; ++i)
	{
		AddObjectToPool();
	}
}

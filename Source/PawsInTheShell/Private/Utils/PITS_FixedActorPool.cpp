// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PawsInTheShell/Public/Utils/PITS_FixedActorPool.h"
#include "Utils/PITS_Globals.h"
#include "Utils/PITS_Logs.h"

AActor* UPITS_FixedActorPool::GetObjectFromPool()
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

	// No available inactive actors found in the pool.
	UE_LOG(LogPITS, Warning, TEXT("No available actors in pool %s"), *GetName());
	return nullptr;

}

void UPITS_FixedActorPool::ReleaseObjectToPool(AActor* Actor)
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

bool UPITS_FixedActorPool::HasAvailableObjectsInPool() const
{
	// Check if there is at least one actor in the pool that is currently 'inactive' (hidden).
	return TheActorPool.ContainsByPredicate([](const TObjectPtr<AActor>& ActorPtr)
	{
		return ActorPtr && ActorPtr->IsHidden();
	});
}

bool UPITS_FixedActorPool::IsObjectPooled(const AActor* Actor) const
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

void UPITS_FixedActorPool::InitializePool(const TSubclassOf<AActor> SpawnableClass, const int32 PoolSize)
{
	// Validate input parameters.
	if (!SpawnableClass)
	{
		UE_LOG(LogPITS, Warning, TEXT("SpawnableClass is not set in %s. Pool won't be initialized."), *GetName());
		return;
	}
	// Store pool configuration.
	ThePoolSize = PoolSize;
	TheSpawnableClass = SpawnableClass;
	
	UWorld* World = GetWorld();
	
	// Guard against missing world context.
	CHECK_PTR_AND_LOG_RETURN(World);

	// Clear any existing pool entries.
	TheActorPool.Empty();

	// Pre-spawn the specified number of actors and add them to the pool.
	for (int32 i = 0; i < ThePoolSize; ++i)
	{
		FActorSpawnParameters SpawnParams;
		// Always spawn even if collisions might overlap during initialization.
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the actor and, if successful, deactivate it for pooling.
		if (AActor* NewActor = World->SpawnActor<AActor>(TheSpawnableClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
		{
			// Ensure the actor is inactive until explicitly acquired.
			NewActor->SetActorEnableCollision(false);
			NewActor->SetActorHiddenInGame(true);
			// Prevent automatic destruction by zeroing lifespan.
			NewActor->SetLifeSpan(0.f);
			TheActorPool.Add(NewActor);
		}
		else
		{
			// Log spawn failures for debugging spawn class / world issues.
			UE_LOG(LogPITS, Warning, TEXT("Failed to spawn actor for pool in %s"), *GetName());
		}
	}
}

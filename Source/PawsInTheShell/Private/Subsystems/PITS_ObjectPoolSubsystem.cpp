// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Subsystems/PITS_ObjectPoolSubsystem.h"

#include "SNegativeActionButton.h"
#include "Interfaces/PITS_PooledObjectInterface.h"
#include "Utils/PITS_FixedActorPool.h"
#include "Utils/PITS_Globals.h"
#include "Utils/PITS_LazyActorPool.h"
#include "Utils/PITS_Logs.h"

void UPITS_ObjectPoolSubsystem::CreateObjectPool(const TSubclassOf<AActor> SpawnableClass, const int32 PoolSize)
{
	// Validate input
	if (!SpawnableClass)
	{
		UE_LOG(LogPITS, Warning, TEXT("CreateObjectPool called with null SpawnableClass"));
		return;
	}

	// Check if pool already exists
	if (HasObjectPool(SpawnableClass))
	{
		UE_LOG(LogPITS, Log, TEXT("Object pool for class %s already exists. Skipping creation."), *GetNameSafe(SpawnableClass));
		return;
	}

	// Create and initialize new pool
	if (UPITS_LazyActorPool* NewPool = NewObject<UPITS_LazyActorPool>(this))
	{
		NewPool->InitializePool(SpawnableClass, PoolSize);
		ActorsPoolMap.Add(SpawnableClass, NewPool);
		UE_LOG(LogPITS, Log, TEXT("Created object pool for class %s with size %d"), *GetNameSafe(SpawnableClass), PoolSize);
	}
}

void UPITS_ObjectPoolSubsystem::AddObjectsToPool(const TSubclassOf<AActor> SpawnableClass, const int32 NumObjects, const bool bCreateIfNotFound)
{
	if (!HasObjectPool(SpawnableClass))
	{
		if (!bCreateIfNotFound)
		{
			UE_LOG(LogPITS, Warning, TEXT("No pool found for class %s. Set bCreateIfNotFound to true to create a new pool before adding objects."), *GetNameSafe(SpawnableClass));	
		}
		else
		{
			UE_LOG(LogPITS, Log, TEXT("No pool found for class %s. Creating new pool before adding objects."), *GetNameSafe(SpawnableClass));
			CreateObjectPool(SpawnableClass, NumObjects);
		}
	}
}

bool UPITS_ObjectPoolSubsystem::HasObjectPool(const TSubclassOf<AActor> SpawnableClass) const
{
	return ActorsPoolMap.Contains(SpawnableClass);
}

AActor* UPITS_ObjectPoolSubsystem::AcquirePooledObject(const TSubclassOf<AActor> SpawnableClass, const FTransform ObjectTransform)
{
	// Ensure pool exists
	if (!HasObjectPool(SpawnableClass)) CreateObjectPool(SpawnableClass);
	
	// Try to get an object from the pool
	if (AActor* PooledActor = GetObjectPool(SpawnableClass)->RequestObjectFromPool())
	{
		PooledActor->SetActorTransform(ObjectTransform);
		// If Actor implements UPITS_PooledObjectInterface, notify that it has been acquired from the pool.
		if (PooledActor->GetClass()->ImplementsInterface(UPITS_PooledObjectInterface::StaticClass()))
		{
			IPITS_PooledObjectInterface::Execute_HandleAcquire(PooledActor);
		}

		return PooledActor;
	}
	return nullptr;
}

void UPITS_ObjectPoolSubsystem::ReleasePooledObject(AActor* Actor)
{
	// Validate input
	CHECK_PTR_AND_LOG_RETURN(Actor);
	
	// Release actor back to its pool
	if (const TSubclassOf<AActor> ActorClass = Actor->GetClass(); HasObjectPool(ActorClass))
	{
		GetObjectPool(ActorClass)->ReleaseObjectToPool(Actor);
		// If Actor implements UPITS_PooledObjectInterface, notify that it has been released from the pool.
		if (Actor->GetClass()->ImplementsInterface(UPITS_PooledObjectInterface::StaticClass()))
		{
			IPITS_PooledObjectInterface::Execute_HandleRelease(Actor);
		}
	}
	else
	{
		UE_LOG(LogPITS, Warning, TEXT("No object pool found for class %s when releasing actor %s"), *GetNameSafe(ActorClass), *GetNameSafe(Actor));
	}
}

UPITS_LazyActorPool* UPITS_ObjectPoolSubsystem::GetObjectPool(const TSubclassOf<AActor> SpawnableClass) const
{
	return ActorsPoolMap.FindRef(SpawnableClass);
}

bool UPITS_ObjectPoolSubsystem::HasAvailablePooledObjects(const TSubclassOf<AActor> SpawnableClass) const
{
	// Check pool existence
	if (!HasObjectPool(SpawnableClass))
	{
		UE_LOG(LogPITS, Warning, TEXT("No object pool found for class %s"), *GetNameSafe(SpawnableClass));
		return false;
	}
	return GetObjectPool(SpawnableClass)->HasAvailableObjectsInPool();
}

bool UPITS_ObjectPoolSubsystem::IsObjectPooled(const AActor* Actor) const
{
	// Validate input
	if (const TSubclassOf<AActor> Class = Actor->GetClass(); HasObjectPool(Class))
	{
		// Check if actor is in the pool and return result
		const UPITS_LazyActorPool* Pool = GetObjectPool(Class);
		return Pool->IsObjectPooled(Actor);
	}
	return false;
}

#if WITH_DEV_AUTOMATION_TESTS
int32 UPITS_ObjectPoolSubsystem::GetPoolSize(const TSubclassOf<AActor> SpawnableClass) const
{
	const UPITS_LazyActorPool* Pool = GetObjectPool(SpawnableClass);
	return Pool->GetPoolSize();
}

void UPITS_ObjectPoolSubsystem::CleanupObjectPool(const TSubclassOf<AActor> SpawnableClass)
{
	if (!HasObjectPool(SpawnableClass)) return;
	TObjectPtr<UPITS_LazyActorPool> Pool;
	
	// remove pool from map
	ActorsPoolMap.RemoveAndCopyValue(SpawnableClass, Pool);
	
	if (HasObjectPool(SpawnableClass))
	{
		UE_LOG(LogPITS, Warning, TEXT("Failed to remove pool for class %s"), *GetNameSafe(SpawnableClass));
	}
	
	// Cleanup pool
	Pool->Empty();
	Pool = nullptr;
}
#endif

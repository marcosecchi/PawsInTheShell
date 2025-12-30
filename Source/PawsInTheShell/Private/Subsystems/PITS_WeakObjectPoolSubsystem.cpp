// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Subsystems/PITS_WeakObjectPoolSubsystem.h"
#include "Interfaces/PITS_PooledObjectInterface.h"
#include "Utils/PITS_Globals.h"
#include "Utils/PITS_Logs.h"
#include "Utils/PITS_FixedActorPool_Weak.h"

void UPITS_WeakObjectPoolSubsystem::CreateObjectPool(const TSubclassOf<AActor> SpawnableClass, const int32 PoolSize)
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
		UE_LOG(LogPITS, Warning, TEXT("Object pool for class %s already exists"), *GetNameSafe(SpawnableClass));
		return;
	}

	// Create and initialize new pool
	if (TUniquePtr<FPITS_FixedActorPool_Weak> NewPool = MakeUnique<FPITS_FixedActorPool_Weak>())
	{
		NewPool->InitializePool(GetWorld(), SpawnableClass, PoolSize);
		ActorsPoolMap.Add(SpawnableClass, MoveTemp(NewPool));
		UE_LOG(LogPITS, Log, TEXT("Created object pool for class %s with size %d"), *GetNameSafe(SpawnableClass), PoolSize);
	}
}

bool UPITS_WeakObjectPoolSubsystem::HasObjectPool(const TSubclassOf<AActor> SpawnableClass) const
{
	return ActorsPoolMap.Contains(SpawnableClass);
}

AActor* UPITS_WeakObjectPoolSubsystem::AcquirePooledObject(const TSubclassOf<AActor> SpawnableClass, const FTransform ObjectTransform)
{
	// Ensure pool exists
	if (!HasObjectPool(SpawnableClass)) CreateObjectPool(SpawnableClass);
	
	// Try to get an object from the pool and return it
	if (AActor* PooledActor = GetObjectPool(SpawnableClass)->GetObjectFromPool())
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

void UPITS_WeakObjectPoolSubsystem::ReleasePooledObject(AActor* Actor)
{
	// Validate input
	CHECK_PTR_AND_LOG_RETURN(Actor);
	
	// Release the actor back to its pool
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

FPITS_FixedActorPool_Weak* UPITS_WeakObjectPoolSubsystem::GetObjectPool(const TSubclassOf<AActor> SpawnableClass) const
{
	// Return the raw pointer owned by the TUniquePtr in the map
	if (const TUniquePtr<FPITS_FixedActorPool_Weak>* Ptr = ActorsPoolMap.Find(SpawnableClass))
	{
		return Ptr->Get();
	}
	return nullptr;
}

bool UPITS_WeakObjectPoolSubsystem::HasAvailablePooledObjects(const TSubclassOf<AActor> SpawnableClass) const
{
	// Check pool existence
	if (!HasObjectPool(SpawnableClass))
	{
		UE_LOG(LogPITS, Warning, TEXT("No object pool found for class %s"), *GetNameSafe(SpawnableClass));
		return false;
	}
	return GetObjectPool(SpawnableClass)->HasAvailableObjectsInPool();
}

bool UPITS_WeakObjectPoolSubsystem::IsObjectPooled(const AActor* Actor) const
{
	// Validate input
	if (const TSubclassOf<AActor> Class = Actor->GetClass(); HasObjectPool(Class))
	{
		// Check if actor is in the pool and return result
		const FPITS_FixedActorPool_Weak* Pool = GetObjectPool(Class);
		return Pool->IsObjectPooled(Actor);
	}
	return false;
}

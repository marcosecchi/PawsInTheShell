// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Subsystems/PITS_ObjectPoolSubsystem.h"
#include "Interfaces/PITS_PooledObjectInterface.h"
#include "Utils/PITS_FixedActorPool.h"
#include "Utils/PITS_Globals.h"
#include "Utils/PITS_Logs.h"

void UPITS_ObjectPoolSubsystem::CreateObjectPool(const TSubclassOf<AActor> SpawnableClass, const int32 PoolSize)
{
	if (!SpawnableClass)
	{
		UE_LOG(LogPITS, Warning, TEXT("CreateObjectPool called with null SpawnableClass"));
		return;
	}

	if (HasObjectPool(SpawnableClass))
	{
		UE_LOG(LogPITS, Warning, TEXT("Object pool for class %s already exists"), *GetNameSafe(SpawnableClass));
		return;
	}

	if (UPITS_FixedActorPool* NewPool = NewObject<UPITS_FixedActorPool>(this))
	{
		NewPool->InitializePool(SpawnableClass, PoolSize);
		ActorsPoolMap.Add(SpawnableClass, NewPool);
		UE_LOG(LogPITS, Log, TEXT("Created object pool for class %s with size %d"), *GetNameSafe(SpawnableClass), PoolSize);
	}
}

bool UPITS_ObjectPoolSubsystem::HasObjectPool(const TSubclassOf<AActor> SpawnableClass) const
{
	return ActorsPoolMap.Contains(SpawnableClass);
}

AActor* UPITS_ObjectPoolSubsystem::AcquirePooledObject(const TSubclassOf<AActor> SpawnableClass, const FTransform ObjectTransform)
{
	if (!HasObjectPool(SpawnableClass)) CreateObjectPool(SpawnableClass);
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

void UPITS_ObjectPoolSubsystem::ReleasePooledObject(AActor* Actor)
{
	CHECK_PTR_AND_LOG_RETURN(Actor);
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

UPITS_FixedActorPool* UPITS_ObjectPoolSubsystem::GetObjectPool(const TSubclassOf<AActor> SpawnableClass) const
{
	return ActorsPoolMap.FindRef(SpawnableClass);
}

bool UPITS_ObjectPoolSubsystem::HasAvailablePooledObjects(const TSubclassOf<AActor> SpawnableClass) const
{
	if (!HasObjectPool(SpawnableClass))
	{
		UE_LOG(LogPITS, Warning, TEXT("No object pool found for class %s"), *GetNameSafe(SpawnableClass));
		return false;
	}
	return GetObjectPool(SpawnableClass)->HasAvailableObjectsInPool();
}

bool UPITS_ObjectPoolSubsystem::IsObjectPooled(const AActor* Actor) const
{
	if (const TSubclassOf<AActor> Class = Actor->GetClass(); HasObjectPool(Class))
	{
		const UPITS_FixedActorPool* Pool = GetObjectPool(Class);
		return Pool->IsObjectPooled(Actor);
	}
	return false;
}

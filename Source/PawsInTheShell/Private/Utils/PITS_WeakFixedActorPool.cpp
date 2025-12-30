// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Utils/PITS_WeakFixedActorPool.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Utils/PITS_Globals.h"
#include "Utils/PITS_Logs.h"

void FPITS_WeakFixedActorPool::InitializePool(UWorld* World, const TSubclassOf<AActor> SpawnableClass, const int32 PoolSize)
{
    // Clear any existing pool entries
    TheActorPool.Reset();
    FreeIndices.Reset();
    ActorToIndex.Empty();

    // Validate input parameters
    if (!World || !SpawnableClass || PoolSize <= 0)
    {
        UE_LOG(LogPITS, Warning, TEXT("Invalid parameters provided to InitializePool. Pool won't be initialized."));
        return;
    }

    // Pre-spawn the specified number of actors and add them to the pool
    TheActorPool.Reserve(PoolSize);
    FreeIndices.Reserve(PoolSize);

	// Always spawn even if collisions might overlap during initialization.
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Spawn actors and add them to the pool
    for (int32 i = 0; i < PoolSize; ++i)
    {
        AActor* NewActor = World->SpawnActor<AActor>(SpawnableClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
        TheActorPool.Add(TWeakObjectPtr<AActor>(NewActor));
        // If spawn succeeded, deactivate the actor and track its index
        if (NewActor)
        {
            // Put the actor into an 'inactive' state by default
            NewActor->SetActorHiddenInGame(true);
            NewActor->SetActorEnableCollision(false);
            NewActor->SetActorTickEnabled(false);

            ActorToIndex.Add(NewActor, i);
            FreeIndices.Push(i);
        }
    }
}

AActor* FPITS_WeakFixedActorPool::GetObjectFromPool()
{
    // Pop indices until we find a valid actor or run out of free indices
    while (FreeIndices.Num() > 0)
    {
        // Try to get a free index
        if (const int32 Index = FreeIndices.Pop(); TheActorPool.IsValidIndex(Index))
        {
            // Check if the weak pointer is still valid
            if (TWeakObjectPtr<AActor> WeakActor = TheActorPool[Index]; WeakActor.IsValid())
            {
                AActor* Actor = WeakActor.Get();
                // Activate actor
                Actor->SetActorHiddenInGame(false);
                Actor->SetActorEnableCollision(true);
                Actor->SetActorTickEnabled(true);
                return Actor;
            }
        }
        // invalid entry: continue popping
    }
    // none available, return nullptr
    return nullptr;
}

void FPITS_WeakFixedActorPool::ReleaseObjectToPool(AActor* Actor)
{
    // Guard against null pointers
    CHECK_PTR_AND_LOG_RETURN(Actor);

    // Find the index of the actor in the pool
    const int32* FoundIndex = ActorToIndex.Find(Actor);
    if (!FoundIndex)
        return;

    // Validate index and ensure it refers to a valid actor
    if (const int32 Index = *FoundIndex; TheActorPool.IsValidIndex(Index))
    {
        // Deactivate actor
        Actor->SetActorHiddenInGame(true);
        Actor->SetActorEnableCollision(false);
        Actor->SetActorTickEnabled(false);

        // Push the index back onto the free stack
        FreeIndices.Push(Index);
    }
}

bool FPITS_WeakFixedActorPool::HasAvailableObjectsInPool() const
{
    // Clean invalid indices lazily is left to user; here just check if any free index refers to a valid actor.
    for (int32 i = FreeIndices.Num() - 1; i >= 0; --i)
    {
        // Check if the weak pointer at this index is still valid
        if (const int32 Index = FreeIndices[i]; TheActorPool.IsValidIndex(Index) && TheActorPool[Index].IsValid())
            return true;
    }
    return false;
}

bool FPITS_WeakFixedActorPool::IsObjectPooled(const AActor* Actor) const
{
    return Actor && ActorToIndex.Contains(const_cast<AActor*>(Actor));
}
// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Utils/PITS_FixedActorPool_Weak.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

void FPITS_FixedActorPool_Weak::InitializePool(UWorld* InWorld, const TSubclassOf<AActor> InSpawnableClass, const int32 InPoolSize)
{
    World = InWorld;
    SpawnableClass = InSpawnableClass;

    ActorPool.Reset();
    FreeIndices.Reset();
    ActorToIndex.Empty();

    if (!World || !SpawnableClass || InPoolSize <= 0)
    {
        return;
    }

    ActorPool.Reserve(InPoolSize);
    FreeIndices.Reserve(InPoolSize);

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 i = 0; i < InPoolSize; ++i)
    {
        AActor* NewActor = World->SpawnActor<AActor>(SpawnableClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
        ActorPool.Add(TWeakObjectPtr<AActor>(NewActor));
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

AActor* FPITS_FixedActorPool_Weak::GetObjectFromPool()
{
    while (FreeIndices.Num() > 0)
    {
        if (const int32 Index = FreeIndices.Pop(); ActorPool.IsValidIndex(Index))
        {
            if (TWeakObjectPtr<AActor> Weak = ActorPool[Index]; Weak.IsValid())
            {
                AActor* Actor = Weak.Get();
                // Activate actor
                Actor->SetActorHiddenInGame(false);
                Actor->SetActorEnableCollision(true);
                Actor->SetActorTickEnabled(true);
                return Actor;
            }
        }
        // invalid entry: continue popping
    }
    // none available
    return nullptr;
}

void FPITS_FixedActorPool_Weak::ReleaseObjectToPool(AActor* Actor)
{
    if (!Actor)
        return;

    const int32* FoundIndex = ActorToIndex.Find(Actor);
    if (!FoundIndex)
        return;

    if (const int32 Index = *FoundIndex; ActorPool.IsValidIndex(Index))
    {
        // Deactivate actor
        Actor->SetActorHiddenInGame(true);
        Actor->SetActorEnableCollision(false);
        Actor->SetActorTickEnabled(false);

        FreeIndices.Push(Index);
    }
}

bool FPITS_FixedActorPool_Weak::HasAvailableObjectsInPool() const
{
    // Clean invalid indices lazily is left to user; here just check if any free index refers to a valid actor.
    for (int32 i = FreeIndices.Num() - 1; i >= 0; --i)
    {
        if (const int32 Index = FreeIndices[i]; ActorPool.IsValidIndex(Index) && ActorPool[Index].IsValid())
            return true;
    }
    return false;
}

bool FPITS_FixedActorPool_Weak::IsObjectPooled(const AActor* Actor) const
{
    return Actor && ActorToIndex.Contains(const_cast<AActor*>(Actor));
}
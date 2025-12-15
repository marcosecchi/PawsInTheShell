// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_EnemySpawner.h"
#include "Utils/PITS_Globals.h"
#include "Utils/PITS_Logs.h"

// Sets default values for the enemy spawner actor.
// The spawner does not tick by default and prepares a shared config object.
APITS_EnemySpawner::APITS_EnemySpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

// Acquire an available enemy from the pool.
// Iterates over the weak pointers in the pool and returns the first valid, hidden enemy.
// When an enemy is acquired it is unhidden and collision is enabled.
APITS_BaseEnemyCharacter* APITS_EnemySpawner::GetEnemyFromPool()
{
    for (TObjectPtr<APITS_BaseEnemyCharacter>& EnemyPtr : EnemyPool)
    {
        if (EnemyPtr)
        {
            // Check whether the enemy is currently 'inactive' (hidden) and can be reused.
            if (APITS_BaseEnemyCharacter* Enemy = EnemyPtr.Get(); Enemy->IsHidden())
            {
                Enemy->SetActorHiddenInGame(false);
                Enemy->SetActorEnableCollision(true);

                UE_LOG(LogPITS, Log, TEXT("Acquired enemy %s from pool in EnemySpawner %s"), *Enemy->GetName(), *GetName());
                return Enemy;
            }
        }
    }

    // No available inactive enemy found in the pool.
    UE_LOG(LogPITS, Warning, TEXT("No available enemies in pool in EnemySpawner %s"), *GetName());
    return nullptr;
}

// Return an enemy back into the pool for reuse.
// Validates the pointer and looks for the matching entry in the pool, then deactivates it.
void APITS_EnemySpawner::ReleaseEnemyToPool(APITS_BaseEnemyCharacter* Enemy)
{
    // Macro to guard against null pointers and log a message if null.
    CHECK_PTR_AND_LOG_RETURN(Enemy);

    for (TObjectPtr<APITS_BaseEnemyCharacter>& EnemyPtr : EnemyPool)
    {
        if (EnemyPtr && EnemyPtr.Get() == Enemy)
        {
            // Mark the enemy as inactive so it can be reused later.
            Enemy->SetActorHiddenInGame(true);
            Enemy->SetActorEnableCollision(false);

            UE_LOG(LogPITS, Log, TEXT("Released enemy %s back to pool in EnemySpawner %s"), *Enemy->GetName(), *GetName());
            return;
        }
    }

    // If we reach here the enemy was not found in this pool.
    UE_LOG(LogPITS, Warning, TEXT("Attempted to release an enemy not from this pool in EnemySpawner %s"), *GetName());
}

bool APITS_EnemySpawner::HasAvailableEnemiesInPool() const
{
    return EnemyPool.ContainsByPredicate([](const TObjectPtr<APITS_BaseEnemyCharacter>& EnemyPtr)
    {
        return EnemyPtr && EnemyPtr->IsHidden();
    });
}

// Called when the game starts or when spawned.
// Ensures proper initialization of the pool at begin play.
void APITS_EnemySpawner::BeginPlay()
{
    Super::BeginPlay();
    InitializePool();
}

// Initialize or refill the enemy pool.
// Spawns PoolSize actors of the configured EnemySpawnableClass, disables them, and stores weak refs.
void APITS_EnemySpawner::InitializePool()
{
    if (!EnemySpawnableClass)
    {
        UE_LOG(LogPITS, Warning, TEXT("EnemySpawnableClass is not set in EnemySpawner %s"), *GetName());
        return;
    }

    UWorld* World = GetWorld();
    // Guard against missing world context.
    CHECK_PTR_AND_LOG_RETURN(World);

    EnemyPool.Empty();

    for (int32 i = 0; i < PoolSize; ++i)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        // Always spawn even if collisions might overlap during initialization.
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        // Spawn the enemy actor and, if successful, deactivate it for pooling.
        if (APITS_BaseEnemyCharacter* NewEnemy = World->SpawnActor<APITS_BaseEnemyCharacter>(EnemySpawnableClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
        {
            // Ensure the actor is inactive until explicitly acquired.
            NewEnemy->SetActorEnableCollision(false);
            NewEnemy->SetActorHiddenInGame(true);
            // Prevent automatic destruction by zeroing lifespan.
            NewEnemy->SetLifeSpan(0.f);
            NewEnemy->SetOwningSpawner(this);
            EnemyPool.Add(NewEnemy);
        }
        else
        {
            // Log spawn failures for debugging spawn class / world issues.
            UE_LOG(LogPITS, Warning, TEXT("Failed to spawn enemy for pool in EnemySpawner %s"), *GetName());
        }
    }
}

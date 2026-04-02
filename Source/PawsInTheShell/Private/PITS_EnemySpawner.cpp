// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_EnemySpawner.h"

#include "PITS_BaseEnemyCharacter.h"
#include "Subsystems/PITS_ObjectPoolSubsystem.h"
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
APITS_BaseEnemyCharacter* APITS_EnemySpawner::AcquireEnemyFromPool(const FTransform Transform) const
{
    if (PoolSubsystem == nullptr)
    {
        UE_LOG(LogPITS, Warning, TEXT("WorldSubsystem is null in EnemySpawner %s"), *GetName());
        return nullptr;
    }
    AActor* Actor = PoolSubsystem->AcquirePooledObject(EnemySpawnableClass, Transform);
    return Cast<APITS_BaseEnemyCharacter>(Actor);
}

// Return an enemy back into the pool for reuse.
// Validates the pointer and looks for the matching entry in the pool, then deactivates it.
void APITS_EnemySpawner::ReleaseEnemyToPool(APITS_BaseEnemyCharacter* Enemy) const
{
    // Macro to guard against null pointers and log a message if null.
    CHECK_PTR_AND_LOG_RETURN(PoolSubsystem);
    PoolSubsystem->ReleasePooledObject(Enemy);
}

void APITS_EnemySpawner::SpawnEnemy()
{
    const APITS_BaseEnemyCharacter* Enemy = AcquireEnemyFromPool(GetSpawnTransform());
    CHECK_PTR_AND_LOG_RETURN(Enemy);
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

    const UWorld* World = GetWorld();
    // Guard against missing world context.
    CHECK_PTR_AND_LOG_RETURN(World);

    PoolSubsystem = World->GetSubsystem<UPITS_ObjectPoolSubsystem>();
    CHECK_PTR_AND_LOG_RETURN(PoolSubsystem);
    
    // Create the object pool in the world subsystem if it doesn't exist.
    if (!PoolSubsystem->HasObjectPool(EnemySpawnableClass))
    {
        PoolSubsystem->CreateObjectPool(EnemySpawnableClass, PoolSize);
    }
}

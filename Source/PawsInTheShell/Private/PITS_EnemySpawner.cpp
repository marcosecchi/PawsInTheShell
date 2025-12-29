// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_EnemySpawner.h"
#include "Subsystems/PITS_WorldSubsystem.h"
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
    if (WorldSubsystem == nullptr)
    {
        UE_LOG(LogPITS, Warning, TEXT("WorldSubsystem is null in EnemySpawner %s"), *GetName());
        return nullptr;
    }
    AActor* Actor = WorldSubsystem->AcquirePooledObject(EnemySpawnableClass, Transform);
    return Cast<APITS_BaseEnemyCharacter>(Actor);
}

// Return an enemy back into the pool for reuse.
// Validates the pointer and looks for the matching entry in the pool, then deactivates it.
void APITS_EnemySpawner::ReleaseEnemyToPool(APITS_BaseEnemyCharacter* Enemy) const
{
    // Macro to guard against null pointers and log a message if null.
    CHECK_PTR_AND_LOG_RETURN(WorldSubsystem);
    WorldSubsystem->ReleasePooledObject(Enemy);
}

void APITS_EnemySpawner::OnEnemyDeath(AActor* DeadEnemy)
{
    APITS_BaseEnemyCharacter* Enemy = Cast<APITS_BaseEnemyCharacter>(DeadEnemy);
    Enemy->OnDeath.RemoveDynamic(this, &APITS_EnemySpawner::OnEnemyDeath);
    ReleaseEnemyToPool(Enemy);
}

void APITS_EnemySpawner::SpawnEnemy()
{
    APITS_BaseEnemyCharacter* Enemy = AcquireEnemyFromPool(GetSpawnTransform());
    CHECK_PTR_AND_LOG_RETURN(Enemy);
    Enemy->OnDeath.AddDynamic(this, &APITS_EnemySpawner::OnEnemyDeath);
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

    WorldSubsystem = World->GetSubsystem<UPITS_WorldSubsystem>();
    CHECK_PTR_AND_LOG_RETURN(WorldSubsystem);
    
    // Create the object pool in the world subsystem if it doesn't exist.
    if (!WorldSubsystem->HasObjectPool(EnemySpawnableClass))
    {
        WorldSubsystem->CreateObjectPool(EnemySpawnableClass, PoolSize);
    }
}

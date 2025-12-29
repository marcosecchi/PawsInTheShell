// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Utils/PITS_FixedActorPool_Weak.h"
#include "PITS_WeakObjectPoolSubsystem.generated.h"

/**
 * World subsystem that manages fixed-size pools of actor instances using a pure C++ pool
 * implementation (FPITS_FixedActorPool_Weak). Each pool is owned by this subsystem via
 * TUniquePtr stored in a TMap keyed by actor class.
 *
 * Implementation notes:
 * - TUniquePtr is not a reflected type, so ActorsPoolMap is intentionally *not* marked with UPROPERTY.
 * - GetObjectPool returns a raw FPITS_FixedActorPool_Weak\* owned by the TUniquePtr in the map (not exposed to reflection).
 * - Ensure the subsystem destructor is defined in the corresponding `.cpp` after including
 *   `Utils/PITS_FixedActorPool_Weak.h` so the pool type is complete when TUniquePtr deletes it.
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_WeakObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	// TUniquePtr is not a reflected type, so we don't add UPROPERTY() here.
	TMap<TSubclassOf<AActor>, TUniquePtr<FPITS_FixedActorPool_Weak>> ActorsPoolMap;

public:
	
	/** Creates an object pool for the specified actor class with the given pool size. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void CreateObjectPool(TSubclassOf<AActor> SpawnableClass, int32 PoolSize = 20);
	
	/** Checks if an object pool exists for the specified actor class. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool HasObjectPool(TSubclassOf<AActor> SpawnableClass) const;
	
	/** Retrieves the object pool for the specified actor class. */
	FPITS_FixedActorPool_Weak* GetObjectPool(const TSubclassOf<AActor> SpawnableClass) const;

	/** Checks if there are available pooled objects for the specified actor class. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool HasAvailablePooledObjects(const TSubclassOf<AActor> SpawnableClass) const;

	/** Checks if the specified actor is managed by any object pool. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool IsObjectPooled(const AActor* Actor) const;
	
	/** Acquires a pooled object of the specified class and sets its transform. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	AActor* AcquirePooledObject(const TSubclassOf<AActor> SpawnableClass, const FTransform ObjectTransform);
	
	/** Releases the specified actor back to its object pool. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void ReleasePooledObject(AActor* Actor);
};

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "PITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest.generated.h"

class APITS_TestPoolableActor;
class APITS_BaseProjectile;
class UPITS_ObjectPoolSubsystem;

/**
 * Functional test that verifies the Object Pool Subsystem ability to acquire (and optionally release)
 * pooled projectile actors. It creates a pool of the specified class/size, acquires one object,
 * validates that its ProjectileMovementComponent is active, and optionally releases it back
 * to the pool — checking that the actor is properly deactivated on release.
 */
UCLASS()
class PAWSINTHESHELL_API APITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest : public AFunctionalTest
{
	GENERATED_BODY()
	
	// Cached reference to the world's Object Pool Subsystem, resolved in PrepareTest().
	UPROPERTY()
	TObjectPtr<UPITS_ObjectPoolSubsystem> PoolSubsystem;
	
	// Number of pre-allocated actors in the pool.
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	int32 PoolSize = 10;
	
	// The projectile class to pool and acquire during the test. Must be set in the editor.
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	TSubclassOf<APITS_TestPoolableActor> SpawnableClass;

	// When true, the test also releases the acquired object back to the pool and verifies
	// that the projectile actor is correctly deactivated after release.
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	bool bReleaseAfterAcquire = false;
	
protected:
	virtual void PrepareTest() override;
	
	virtual void StartTest() override;
};

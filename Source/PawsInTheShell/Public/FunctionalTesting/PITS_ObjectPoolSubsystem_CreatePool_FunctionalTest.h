// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "PITS_ObjectPoolSubsystem_CreatePool_FunctionalTest.generated.h"

class UPITS_ObjectPoolSubsystem;
class APITS_BaseEnemyCharacter;

/**
 * Functional tests that verifies the pool system base operations actually work as expected.
 * It creates a pool of the specified class/size, checks that the pool was created and contains the expected number of pre-allocated actors,
 * and then cleans up the pool before finishing.
 */
UCLASS()
class PAWSINTHESHELL_API APITS_ObjectPoolSubsystem_CreatePool_FunctionalTest : public AFunctionalTest
{
	GENERATED_BODY()

	// Cached reference to the world's Object Pool Subsystem, resolved in PrepareTest().
	UPROPERTY()
	TObjectPtr<UPITS_ObjectPoolSubsystem> PoolSubsystem;
	
	// Number of pre-allocated actors in the pool.
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	int32 PoolSize = 10;
	
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	TSubclassOf<AActor> SpawnableClass;

protected:
	virtual void PrepareTest() override;
	
	virtual void StartTest() override;
};

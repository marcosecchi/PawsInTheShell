// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Tests/FunctionalTesting/PITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest.h"
#include "PITS_BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Subsystems/PITS_ObjectPoolSubsystem.h"

void APITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest::PrepareTest()
{
	Super::PrepareTest();
	PoolSubsystem = GetWorld()->GetSubsystem<UPITS_ObjectPoolSubsystem>();
}

void APITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest::StartTest()
{
	Super::StartTest();

	if (SpawnableClass == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("SpawnableClass is null"));
		return;
	}

	if (PoolSubsystem == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("UPITS_ObjectPoolSubsystem is null"));
		return;
	}

	if (!PoolSubsystem->HasObjectPool(SpawnableClass))
	{
		PoolSubsystem->CreateObjectPool(SpawnableClass, PoolSize);
	}

	AActor* Enemy = PoolSubsystem->AcquirePooledObject(SpawnableClass, FTransform::Identity);
	APITS_BaseProjectile* PooledEnemy = Cast<APITS_BaseProjectile>(Enemy);
	if (PooledEnemy == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Failed to acquire pooled object"));
		return;
	}
	const UProjectileMovementComponent* ProjectileMovement = PooledEnemy->GetProjectileMovement();
	if (!ProjectileMovement->IsActive())
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Projectile movement is not active"));
	}

	if (bReleaseAfterAcquire)
	{
		PoolSubsystem->ReleasePooledObject(PooledEnemy);
		if (ProjectileMovement->IsActive())
		{
			FinishTest(EFunctionalTestResult::Failed, TEXT("Projectile movement should be inactive after release"));
		}
	}	
//		RegisterAutoDestroyActor(Actor);
	PoolSubsystem->CleanupObjectPool(SpawnableClass);
	FinishTest(EFunctionalTestResult::Succeeded, TEXT("Pool subsystem functional test passed"));
}

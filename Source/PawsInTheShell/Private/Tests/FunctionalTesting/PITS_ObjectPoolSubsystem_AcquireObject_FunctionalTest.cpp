// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Tests/FunctionalTesting/PITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest.h"
#include "Subsystems/PITS_ObjectPoolSubsystem.h"
#include "Tests/Helpers/PITS_TestPoolableActor.h"

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
	APITS_TestPoolableActor* PooledObject = Cast<APITS_TestPoolableActor>(Enemy);
	if (PooledObject == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Failed to acquire pooled object"));
		return;
	}
	if (PooledObject->IsHidden())
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Actor has not been correctly acquired from pool"));
	}

	if (bReleaseAfterAcquire)
	{
		PoolSubsystem->ReleasePooledObject(PooledObject);
		if (!PooledObject->IsHidden())
		{
			FinishTest(EFunctionalTestResult::Failed, TEXT("Actor has not been correctly released from pool"));
		}
	}	
//		RegisterAutoDestroyActor(Actor);
	PoolSubsystem->CleanupObjectPool(SpawnableClass);
	FinishTest(EFunctionalTestResult::Succeeded, TEXT("Pool subsystem functional test passed"));
}

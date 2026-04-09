// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PawsInTheShell/Public/Tests/FunctionalTesting/Subsystems/PITS_ObjectPoolSubsystem_CreatePool_FunctionalTest.h"
#include "Subsystems/PITS_ObjectPoolSubsystem.h"
#include "Utils/PITS_Logs.h"

void APITS_ObjectPoolSubsystem_CreatePool_FunctionalTest::PrepareTest()
{
	Super::PrepareTest();
	PoolSubsystem = GetWorld()->GetSubsystem<UPITS_ObjectPoolSubsystem>();
}

void APITS_ObjectPoolSubsystem_CreatePool_FunctionalTest::StartTest()
{
	Super::StartTest();
	
	if (PoolSubsystem == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("UPITS_ObjectPoolSubsystem is null"));
		return;
	}

	if (SpawnableClass == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("SpawnableClass is null"));
		return;
	}

	PoolSubsystem->CleanupObjectPool(SpawnableClass);
	PoolSubsystem->CreateObjectPool(SpawnableClass, PoolSize);
	
	if (PoolSubsystem->GetPoolSize(SpawnableClass) != PoolSize)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Pool size does not match expected value"));
		return;
	}
	if (PoolSubsystem->GetPoolSize(SpawnableClass) == 0)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Pool size should have at least one object"));
		return;
	}
	
	FinishTest(EFunctionalTestResult::Succeeded, TEXT("Pool subsystem functional test passed"));
}

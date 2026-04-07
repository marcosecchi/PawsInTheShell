// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PawsInTheShell/Public/FunctionalTesting/PITS_ObjectPoolSubsystem_CreatePool_FunctionalTest.h"
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
	else
	{
		UE_LOG(LogPITS, Warning, TEXT("Pool for class %s already exists at test start"), *GetNameSafe(SpawnableClass));
	}
	
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
	
	PoolSubsystem->CleanupObjectPool(SpawnableClass);
	FinishTest(EFunctionalTestResult::Succeeded, TEXT("Pool subsystem functional test passed"));
}

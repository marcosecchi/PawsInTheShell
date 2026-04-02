// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "PITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest.generated.h"

class APITS_BaseProjectile;
class UPITS_ObjectPoolSubsystem;

UCLASS()
class PAWSINTHESHELL_API APITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest : public AFunctionalTest
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UPITS_ObjectPoolSubsystem> PoolSubsystem;
	
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	int32 PoolSize = 10;
	
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	TSubclassOf<APITS_BaseProjectile> SpawnableClass;

protected:
	virtual void PrepareTest() override;
	
	virtual void StartTest() override;
};

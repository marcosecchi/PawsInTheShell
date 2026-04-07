// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/PITS_PooledObjectInterface.h"
#include "PITS_TestPoolableActor.generated.h"

/**
 * 
 */
UCLASS()
class PAWSINTHESHELL_API APITS_TestPoolableActor : public AActor, public IPITS_PooledObjectInterface
{
	GENERATED_BODY()
	
public:
	APITS_TestPoolableActor();

protected:
	virtual void BeginPlay() override;

// IPITS_PooledObjectInterface implementations
public:
	virtual void HandleAcquire_Implementation() override;
	virtual void HandleRelease_Implementation() override;

};

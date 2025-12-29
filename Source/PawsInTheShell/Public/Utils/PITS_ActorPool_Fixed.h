// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PITS_ActorPool_Fixed.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PAWSINTHESHELL_API UPITS_ActorPool_Fixed : public UObject
{
	GENERATED_BODY()

	TArray<TObjectPtr<AActor>> ActorPool;

	UPROPERTY()
	TSubclassOf<AActor> TheSpawnableClass;

	UPROPERTY()
	int32 ThePoolSize = 10;

public:
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|PoolSystem")
	AActor* GetObjectFromPool();
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|PoolSystem")
	void ReleaseObjectToPool(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|PoolSystem")
	bool HasAvailableObjectsInPool() const;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|PoolSystem")
	bool IsObjectPooled(const AActor* Actor) const;

	void InitializePool(TSubclassOf<AActor> SpawnableClass, int32 PoolSize);
};

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_WorldSubsystem.h"
#include "PITS_ObjectPoolSubsystem.generated.h"

class UPITS_FixedActorPool;

/**
 * 
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_ObjectPoolSubsystem : public UPITS_WorldSubsystem
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<TSubclassOf<AActor>, TObjectPtr<class UPITS_FixedActorPool>> ActorsPoolMap;

public:
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void CreateObjectPool(TSubclassOf<AActor> SpawnableClass, int32 PoolSize = 20);
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool HasObjectPool(TSubclassOf<AActor> SpawnableClass) const;
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	UPITS_FixedActorPool* GetObjectPool(const TSubclassOf<AActor> SpawnableClass) const;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool HasAvailablePooledObjects(const TSubclassOf<AActor> SpawnableClass) const;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool IsObjectPooled(const AActor* Actor) const;
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	AActor* AcquirePooledObject(const TSubclassOf<AActor> SpawnableClass, const FTransform ObjectTransform);
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void ReleasePooledObject(AActor* Actor);
};

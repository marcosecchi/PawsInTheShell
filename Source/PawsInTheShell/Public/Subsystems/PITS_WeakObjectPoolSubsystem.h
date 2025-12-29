// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Utils/PITS_FixedActorPool_Weak.h"
#include "PITS_WeakObjectPoolSubsystem.generated.h"

//class FPITS_FixedActorPool_Weak;

/**
 * 
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_WeakObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	// TUniquePtr is not a reflected type, so we don't add UPROPERTY() here.
	TMap<TSubclassOf<AActor>, TUniquePtr<FPITS_FixedActorPool_Weak>> ActorsPoolMap;

public:
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void CreateObjectPool(TSubclassOf<AActor> SpawnableClass, int32 PoolSize = 20);
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool HasObjectPool(TSubclassOf<AActor> SpawnableClass) const;
	
	//UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	FPITS_FixedActorPool_Weak* GetObjectPool(const TSubclassOf<AActor> SpawnableClass) const;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool HasAvailablePooledObjects(const TSubclassOf<AActor> SpawnableClass) const;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	bool IsObjectPooled(const AActor* Actor) const;
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	AActor* AcquirePooledObject(const TSubclassOf<AActor> SpawnableClass, const FTransform ObjectTransform);
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void ReleasePooledObject(AActor* Actor);
};

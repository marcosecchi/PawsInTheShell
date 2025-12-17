// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BaseEnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "Subsystems/PITS_WorldSubsystem.h"
#include "PITS_EnemySpawner.generated.h"

UCLASS(Abstract)
class PAWSINTHESHELL_API APITS_EnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	APITS_EnemySpawner();

	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	int32 PoolSize = 10;
	
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	TSubclassOf<APITS_BaseEnemyCharacter> EnemySpawnableClass;
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|PoolSystem")
	void SpawnEnemy();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="PawsInTheShell|PoolSystem")
	FTransform GetSpawnTransform();

protected:
	virtual void BeginPlay() override;

	void InitializePool();
	
	UPROPERTY()
	TObjectPtr<UPITS_WorldSubsystem> WorldSubsystem;

	UFUNCTION()
	APITS_BaseEnemyCharacter* GetEnemyFromPool() const;
	
	UFUNCTION()
	void ReleaseEnemyToPool(APITS_BaseEnemyCharacter* Enemy) const;
	
	UFUNCTION()
	void OnEnemyDeath(AActor* DeadEnemy);

};

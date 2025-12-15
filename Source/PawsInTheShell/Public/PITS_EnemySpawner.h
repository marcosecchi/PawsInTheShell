// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BaseEnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "PITS_EnemySpawner.generated.h"

UCLASS()
class PAWSINTHESHELL_API APITS_EnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APITS_EnemySpawner();

	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	int32 PoolSize = 10;
	
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|PoolSystem")
	TSubclassOf<APITS_BaseEnemyCharacter> EnemySpawnableClass;
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|PoolSystem")
	APITS_BaseEnemyCharacter* GetEnemyFromPool();
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|PoolSystem")
	void ReleaseEnemyToPool(APITS_BaseEnemyCharacter* Enemy);
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|PoolSystem")
	bool HasAvailableEnemiesInPool() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void InitializePool();
	
	TArray<TObjectPtr<APITS_BaseEnemyCharacter>> EnemyPool;
};

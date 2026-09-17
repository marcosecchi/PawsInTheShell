// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BaseCharacter.h"
#include "PITS_BaseEnemyCharacter.generated.h"

class APITS_EnemySpawner;

UCLASS(Abstract, BlueprintType, Blueprintable)
class PAWSINTHESHELL_API APITS_BaseEnemyCharacter : public APITS_BaseCharacter
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<APITS_EnemySpawner> OwningSpawner;
	
public:
	APITS_BaseEnemyCharacter();
	
	/** Handles shoot inputs. Must be implemented by derived classes to define the character's shooting behavior. */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell")
	virtual void Shoot(AActor* Target) {};
	
	void SetOwningSpawner(APITS_EnemySpawner* Spawner) { OwningSpawner = Spawner; }
};

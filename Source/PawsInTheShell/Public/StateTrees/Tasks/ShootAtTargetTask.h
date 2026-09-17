// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "UObject/Object.h"
#include "ShootAtTargetTask.generated.h"

class APITS_BaseEnemyCharacter;

USTRUCT(BlueprintType)
struct FShootAtTargetTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<APITS_BaseEnemyCharacter> OwnerCharacter = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<AActor> Target = nullptr;
};

/**
 * A task that will start the AI shooting sequence
 */
USTRUCT(BlueprintType, Category="Paws in the Shell", meta=(DisplayName="Shoot at Target"))
struct PAWSINTHESHELL_API FShootAtTargetTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FShootAtTargetTaskInstanceData;
	virtual const UScriptStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

};

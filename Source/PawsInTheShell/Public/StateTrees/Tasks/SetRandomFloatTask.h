// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "UObject/Object.h"
#include "SetRandomFloatTask.generated.h"

class APITS_BaseEnemyCharacter;

USTRUCT(BlueprintType)
struct FSetRandomFloatTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float MinValue = 0.f;
 
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float MaxValue = 1.f;
 
	UPROPERTY(EditAnywhere, Category = "Output")
	float OutValue = 0.f;

};

/**
 * A task that sets a random float value.
 */
USTRUCT(BlueprintType, Category="Paws in the Shell", meta=(DisplayName="Set Random Float"))
struct PAWSINTHESHELL_API FSetRandomFloatTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FSetRandomFloatTaskInstanceData;
	virtual const UScriptStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};

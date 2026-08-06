// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "UObject/Object.h"
#include "CheckDistanceFromPlayerTask.generated.h"

USTRUCT(BlueprintType)
struct FCheckDistanceFromPlayerTaskInstanceData
{
	GENERATED_BODY()
	
	TWeakObjectPtr<APawn> CachedPlayerPawn = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AActor> OwnerActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	float DetectionDistance = 1500.f;
 
	UPROPERTY(EditAnywhere, Category = "Output")
	float PlayerDistance = 0.f;
 
	UPROPERTY(EditAnywhere, Category = "Output")
	bool bIsInRange = false;
};

/**
 * Task to check the distance from the player
 */
USTRUCT(BlueprintType, Category="Paws in the Shell", meta=(DisplayName="Check Distance from Player"))
struct PAWSINTHESHELL_API FCheckDistanceFromPlayerTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FCheckDistanceFromPlayerTaskInstanceData;
	virtual const UScriptStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

};

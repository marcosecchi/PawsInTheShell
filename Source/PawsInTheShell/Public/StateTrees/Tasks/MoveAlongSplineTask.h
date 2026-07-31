// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeTaskBase.h"
#include "UObject/Object.h"
#include "MoveAlongSplineTask.generated.h"

class USplineComponent;

/**
 * Data that will be unique to every instance of this task in the tree.
 */
USTRUCT(BlueprintType)
struct FMoveAlongSplineTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AActor> Owner;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	float Duration = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Input")
	bool bShouldRotateToDirection = true;

	UPROPERTY(EditAnywhere, Category = "Input")
	bool bReverseMovement = false;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<USplineComponent> SplineRef = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<USceneComponent> MovingComponentRef = nullptr;
	
	UPROPERTY()
	float Position = 0.f;
	
	UPROPERTY()
	float TimePassed = 0.f;
};

/**
 *  Task that will move a Scene Component along a spline during a predefined time
 */
USTRUCT(BlueprintType, meta=(DisplayName="Move Along Spline"))
struct PAWSINTHESHELL_API FMoveAlongSplineTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FMoveAlongSplineTaskInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	
	FMoveAlongSplineTask()
	{
		bShouldCallTick = true;
	}
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif

};

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "StateTrees/Tasks/MoveAlongSplineTask.h"
#include "Components/SplineComponent.h"

EStateTreeRunStatus FMoveAlongSplineTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!IsValid(InstanceData.SplineRef)) return EStateTreeRunStatus::Failed;
	if (!IsValid(InstanceData.MovingComponentRef)) return EStateTreeRunStatus::Failed;
	
	InstanceData.TimePassed += DeltaTime;
	
	if (InstanceData.TimePassed >= InstanceData.Duration)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	const float SplineLength = InstanceData.SplineRef->GetSplineLength();
	float PositionAlongSpline = InstanceData.TimePassed / InstanceData.Duration;
	if (InstanceData.bReverseMovement)
	{
		PositionAlongSpline = 1.f - PositionAlongSpline;
	}
	PositionAlongSpline *= SplineLength;
	
	const FTransform CompTransform = InstanceData.SplineRef->GetTransformAtDistanceAlongSpline(PositionAlongSpline, ESplineCoordinateSpace::World);
	
	const FVector CompLocation = CompTransform.GetLocation();
	InstanceData.MovingComponentRef->SetWorldLocation(CompLocation);
	
	if (InstanceData.bShouldRotateToDirection)
	{
		const FRotator CompRotation = CompTransform.GetRotation().Rotator();
		InstanceData.MovingComponentRef->SetWorldRotation(CompRotation);
	}
	
	return EStateTreeRunStatus::Running;
}

FText FMoveAlongSplineTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return FText::FromString("<b>Move Along Spline</b>");
}

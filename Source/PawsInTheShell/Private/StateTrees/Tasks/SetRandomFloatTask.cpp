// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "StateTrees/Tasks/SetRandomFloatTask.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSetRandomFloatTask::EnterState(FStateTreeExecutionContext& Context,
                                                    const FStateTreeTransitionResult& Transition) const
{
	// Using structured binding to unpack the instance data
	auto& [MinValue, MaxValue, OutValue] = Context.GetInstanceData(*this);
	OutValue = FMath::FRandRange(MinValue, MaxValue);

	return EStateTreeRunStatus::Succeeded;
}

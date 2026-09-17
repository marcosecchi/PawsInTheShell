// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "StateTrees/Tasks/ShootAtTargetTask.h"
#include "PITS_BaseEnemyCharacter.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FShootAtTargetTask::EnterState(FStateTreeExecutionContext& Context,
                                                   const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.OwnerCharacter) return EStateTreeRunStatus::Failed;

	InstanceData.OwnerCharacter->Shoot(InstanceData.Target);
	
	return EStateTreeRunStatus::Succeeded;
}

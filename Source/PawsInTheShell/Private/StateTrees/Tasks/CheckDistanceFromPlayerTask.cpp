// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "StateTrees/Tasks/CheckDistanceFromPlayerTask.h"
#include "StateTreeExecutionContext.h"
#include "Kismet/GameplayStatics.h"

EStateTreeRunStatus FCheckDistanceFromPlayerTask::EnterState(FStateTreeExecutionContext& Context,
                                                             const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.OwnerActor) return EStateTreeRunStatus::Failed;

	InstanceData.CachedPlayerPawn = UGameplayStatics::GetPlayerPawn(InstanceData.OwnerActor, 0);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FCheckDistanceFromPlayerTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.OwnerActor) return EStateTreeRunStatus::Failed;
	
	APawn* PlayerPawn = InstanceData.CachedPlayerPawn.Get();
	
	// Fallback action executed if the player was destroyed/respawned
	if (!PlayerPawn)
	{
		PlayerPawn = UGameplayStatics::GetPlayerPawn(InstanceData.OwnerActor, 0);
		InstanceData.CachedPlayerPawn = PlayerPawn;
	}
	
	if (PlayerPawn)
	{
		const float DistanceSq = FVector::DistSquared(InstanceData.OwnerActor->GetActorLocation(), PlayerPawn->GetActorLocation());
		
		InstanceData.PlayerDistance = FMath::Sqrt(DistanceSq);
		InstanceData.bIsInRange = (DistanceSq <= FMath::Square(InstanceData.DetectionDistance));
	}
	else
	{
		// Handle "No Player" scenario
		InstanceData.PlayerDistance = UE_MAX_FLT;
		InstanceData.bIsInRange = false;
	}
 
	return EStateTreeRunStatus::Running;
}

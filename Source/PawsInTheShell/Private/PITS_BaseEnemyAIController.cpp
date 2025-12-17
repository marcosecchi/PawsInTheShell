// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_BaseEnemyAIController.h"

#include "PITS_BaseEnemyCharacter.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"

APITS_BaseEnemyAIController::APITS_BaseEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// create the StateTree component
	StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAI"));

	// create the AI perception component. It will be configured in BP
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	// subscribe to the AI perception delegates
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &APITS_BaseEnemyAIController::OnPerceptionUpdated);
	AIPerception->OnTargetPerceptionForgotten.AddDynamic(this, &APITS_BaseEnemyAIController::OnPerceptionForgotten);
}

void APITS_BaseEnemyAIController::SetCurrentTarget(APITS_BasePlayerCharacter* Target)
{
	TargetCharacter = Target;
}

void APITS_BaseEnemyAIController::ClearCurrentTarget()
{
	TargetCharacter = nullptr;
}

void APITS_BaseEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// ensure we're possessing an NPC
	if (APITS_BaseEnemyCharacter* Enemy = Cast<APITS_BaseEnemyCharacter>(InPawn))
	{
		// subscribe to the pawn's OnDeath delegate
		Enemy->OnDeath.AddDynamic(this, &APITS_BaseEnemyAIController::OnDeath);
	}
}

void APITS_BaseEnemyAIController::OnDeath(AActor* DeadActor)
{
	// stop movement
	GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::UserAbort);

	// stop StateTree logic
	StateTreeAI->StopLogic(FString(""));

	// unpossess the pawn
	UnPossess();

	// destroy this controller
	// TODO: Handle controller pooling instead of destroying it
	Destroy();
}

void APITS_BaseEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// pass the data to the StateTree delegate hook
	OnShooterPerceptionUpdated.ExecuteIfBound(Actor, Stimulus);
}

void APITS_BaseEnemyAIController::OnPerceptionForgotten(AActor* Actor)
{
	// pass the data to the StateTree delegate hook
	OnShooterPerceptionForgotten.ExecuteIfBound(Actor);
}

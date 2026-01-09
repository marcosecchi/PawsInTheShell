// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "PITS_BasePlayerCharacter.h"
#include "PITS_BaseEnemyAIController.generated.h"

class UStateTreeAIComponent;
class UAIPerceptionComponent;
struct FAIStimulus;

DECLARE_DELEGATE_TwoParams(FShooterPerceptionUpdatedSignature, AActor*, const FAIStimulus&);
DECLARE_DELEGATE_OneParam(FShooterPerceptionForgottenSignature, AActor*);

UCLASS(Abstract)
class PAWSINTHESHELL_API APITS_BaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

	/** Runs the behavior StateTree for this NPC */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStateTreeAIComponent* StateTreeAI;

	/** Detects other actors through sight, hearing and other senses */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerception;

public:
	// Sets default values for this actor's properties
	APITS_BaseEnemyAIController();

	/** Called when an AI perception has been updated. StateTree task delegate hook */
	FShooterPerceptionUpdatedSignature OnShooterPerceptionUpdated;

	/** Called when an AI perception has been forgotten. StateTree task delegate hook */
	FShooterPerceptionForgottenSignature OnShooterPerceptionForgotten;

	/** Sets the targeted enemy */
	void SetCurrentTarget(APITS_BasePlayerCharacter* Target);

	/** Clears the targeted enemy */
	void ClearCurrentTarget();

	/** Returns the targeted enemy */
	APITS_BasePlayerCharacter* GetCurrentTarget() const { return TargetCharacter; };

protected:
	/** Enemy currently being targeted */
	UPROPERTY()
	TObjectPtr<APITS_BasePlayerCharacter> TargetCharacter;

	/** Pawn initialization */
	virtual void OnPossess(APawn* InPawn) override;
	
	/** Pawn cleanup */
	virtual void OnUnPossess() override;

	/** Called when the possessed pawn dies */
	UFUNCTION()
	void OnDeath(AActor* DeadActor);

	/** Called when the AI perception component updates a perception on a given actor */
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	/** Called when the AI perception component forgets a given actor */
	UFUNCTION()
	void OnPerceptionForgotten(AActor* Actor);

};

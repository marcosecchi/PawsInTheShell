// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_SafeZone.h"
#include "PITS_RegenerationZone.generated.h"

UCLASS()
class PAWSINTHESHELL_API APITS_RegenerationZone : public APITS_SafeZone
{
	GENERATED_BODY()

protected:
	virtual void HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) override;
	virtual void HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor) override;

	UPROPERTY(EditDefaultsOnly, Category="Health", meta=(ToolTip="Amount of health to regenerate per second"))
	float RegenerationAmount = 1.0f;

	UPROPERTY()
	TArray<AActor*> RegeneratingActors;

	/** Timer to regenerate health */
	FTimerHandle RegenerationTimer;

	/** Gameplay cleanup */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void RegenerateHealth();

	UFUNCTION()
	void StopRegenerating();
	
};

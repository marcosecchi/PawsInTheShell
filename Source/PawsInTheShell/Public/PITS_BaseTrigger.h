// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BasePlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "PITS_BaseTrigger.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * Base trigger class for pickups and triggerable zones
 */
UCLASS(Abstract)
class PAWSINTHESHELL_API APITS_BaseTrigger : public AActor
{
	GENERATED_BODY()

	/** Collision sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereCollision;

	/** Pickup mesh. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

public:
	// Sets default values for this actor's properties
	APITS_BaseTrigger();

protected:

	/** Gameplay cleanup */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Handles collision overlap */
	UFUNCTION()
	virtual void HandleActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when it's time to respawn this pickup */
	void Respawn();

	/** Time to wait before respawning this pickup */
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|Pickup")
	bool bWillRespawn = true;

	/** Time to wait before respawning this pickup */
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|Pickup", meta = (ClampMin = 0, ClampMax = 120, Units = "s", EditCondition = "bWillRespawn", EditConditionHides))
	float RespawnTime = 4.0f;

	/** Timer to respawn the pickup */
	FTimerHandle RespawnTimer;

	/** Implement this function to handle the pickup logic */
	UFUNCTION(BlueprintNativeEvent, Category="PawsInTheShell|Pickup")
	void HandleTrigger(APITS_BasePlayerCharacter* OverlappedCharacter);
	virtual void HandleTrigger_Implementation(APITS_BasePlayerCharacter* OverlappedCharacter) PURE_VIRTUAL(APITS_BasePickup::HandlePickup_Implementation);
	
	/** Implement this function to handle respawn logic (Vfx, Sfx, etc.) */
	UFUNCTION(BlueprintNativeEvent, Category="PawsInTheShell|Pickup")
	void HandleRespawn();
	virtual void HandleRespawn_Implementation() PURE_VIRTUAL(APITS_BasePickup::HandleRespawn_Implementation);
	
private:
	/** Activate or Deactivate the pickup */
	void SetTriggerActive(bool bIsActive);
};

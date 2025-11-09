// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BasePlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "PITS_BasePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * Base Pickup class
 */
UCLASS(Abstract)
class PAWSINTHESHELL_API APITS_BasePickup : public AActor
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
	APITS_BasePickup();

protected:

	/** Gameplay cleanup */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Handles collision overlap */
	UFUNCTION()
	virtual void HandleActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when it's time to respawn this pickup */
	void RespawnPickup();
	
	/** Time to wait before respawning this pickup */
	UPROPERTY(EditAnywhere, Category="Pickup", meta = (ClampMin = 0, ClampMax = 120, Units = "s"))
	float RespawnTime = 4.0f;

	/** Timer to respawn the pickup */
	FTimerHandle RespawnTimer;

	/** Implement this function to handle the pickup logic */
	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void HandlePickup(APITS_BasePlayerCharacter* OverlappedCharacter);
	
	/** Implement this function to handle respawn logic (Vfx, Sfx, etc.) */
	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void HandleRespawn();

private:
	/** Activate or Deactivate the pickup */
	void SetPickupActive(bool bIsActive);
};

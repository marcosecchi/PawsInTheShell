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
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|Pickup")
	bool bWillRespawn = true;

	/** Time to wait before respawning this pickup */
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|Pickup", meta = (ClampMin = 0, ClampMax = 120, Units = "s", EditCondition = "bWillRespawn", EditConditionHides))
	float RespawnTime = 4.0f;

	/** Timer to respawn the pickup */
	FTimerHandle RespawnTimer;

	/** Implement this function to handle the pickup logic */
	UFUNCTION(BlueprintNativeEvent, Category="PawsInTheShell|Pickup")
	void HandlePickup(APITS_BasePlayerCharacter* OverlappedCharacter);
	virtual void HandlePickup_Implementation(APITS_BasePlayerCharacter* OverlappedCharacter) PURE_VIRTUAL(APITS_BasePickup::HandlePickup_Implementation);
	
	/** Implement this function to handle respawn logic (Vfx, Sfx, etc.) */
	UFUNCTION(BlueprintNativeEvent, Category="PawsInTheShell|Pickup")
	void HandleRespawn();
	virtual void HandleRespawn_Implementation() PURE_VIRTUAL(APITS_BasePickup::HandleRespawn_Implementation);
	
private:
	/** Activate or Deactivate the pickup */
	void SetPickupActive(bool bIsActive);
};

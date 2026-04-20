// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PITS_PooledObjectInterface.h"
#include "PITS_BaseProjectile.generated.h"

class UNiagaraSystem;
class UBoxComponent;
class UProjectileMovementComponent;
class UDamageType;
class UPrimitiveComponent;

UCLASS(Abstract)
class PAWSINTHESHELL_API APITS_BaseProjectile : public AActor, public IPITS_PooledObjectInterface
{
	GENERATED_BODY()
	
	/** Provides collision detection for the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> CollisionComponent;

	/** Handles movement for the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	/** Handles movement for the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> VfxHit;

protected:
	/** Data table row containing projectile properties */
	UPROPERTY(EditDefaultsOnly, Category="PawsInTheShell|Stats")
	FDataTableRowHandle ProjectileDataType;

	UPROPERTY()
	FText ProjectileName = FText::FromString("DefaultProjectile");

	/** Type of damage to apply. Can be used to represent specific types of damage such as fire, explosion, etc. */
	UPROPERTY()
	TSubclassOf<UDamageType> HitDamageType;

	/** Physics force to apply on hit */
	UPROPERTY()
	float PhysicsForce = 100.0f;

	/** Damage to apply on hit */
	UPROPERTY()
	float HitDamage = 25.0f;

	/** If true, the projectile can damage the character that shot it */
	// TODO: Change with Damage Players
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|Projectile")
	bool bDamageOwner = false;

	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	APITS_BaseProjectile();
	
	FText GetProjectileName() const { return ProjectileName; }	

protected:
	virtual void BeginPlay() override;

	/** Handles collision */
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	/** Processes a projectile hit for the given actor */
	void ProcessHit(AActor* HitActor, UPrimitiveComponent* HitComp, const FVector& HitLocation, const FVector& HitDirection);

	/** Passes control to Blueprint to implement any effects on hit. */
	UFUNCTION(BlueprintImplementableEvent, Category="PawsInTheShell|Projectile", meta = (DisplayName = "On Projectile Hit"))
	void HandleProjectileHit(const FHitResult& Hit);

	/** Called from the destruction timer to destroy this projectile */
	void OnHitComplete();

// IPITS_PooledObjectInterface implementations
public:
	virtual void HandleAcquire_Implementation() override;
	virtual void HandleRelease_Implementation() override;
	
// Component getters
	FORCEINLINE UBoxComponent* GetCollisionComponent() const { return CollisionComponent; }
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

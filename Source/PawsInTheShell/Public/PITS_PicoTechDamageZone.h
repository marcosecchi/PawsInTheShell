// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PITS_PicoTechDamageZone.generated.h"

class UArrowComponent;
class USphereComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup="PawsInTheShell")
class PAWSINTHESHELL_API APITS_PicoTechDamageZone : public AActor
{
	GENERATED_BODY()

public:
	APITS_PicoTechDamageZone();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> DamageZoneVolume;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|Damage", meta=(ToolTip="Amount of damage provoked per second"))
	float DamageAmount = 1.0f;

	UPROPERTY()
	TArray<AActor*> DamagedActors;

	UFUNCTION()
	virtual void HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	virtual void HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	/** Timer to regenerate health */
	FTimerHandle DamageTimer;

	/** Gameplay cleanup */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void ProvokeDamage();

	UFUNCTION()
	void StopDamaging();

};

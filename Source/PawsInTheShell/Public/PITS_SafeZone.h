// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PITS_SafeZone.generated.h"

class UArrowComponent;
class USphereComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup="PawsInTheShell")
class PAWSINTHESHELL_API APITS_SafeZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APITS_SafeZone();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> SafeZoneVolume;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UFUNCTION()
	virtual void HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	virtual void HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

};

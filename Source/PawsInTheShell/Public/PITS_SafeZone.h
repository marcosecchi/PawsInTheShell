// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PITS_SafeZone.generated.h"

class UArrowComponent;
class UBoxComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup="Pawns In The Shell")
class PAWSINTHESHELL_API APITS_SafeZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APITS_SafeZone();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<UBoxComponent> SafeZoneVolume;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> Arrow;

	UFUNCTION()
	virtual void HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	virtual void HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

};

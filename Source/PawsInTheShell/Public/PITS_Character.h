// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PITS_Character.generated.h"

UCLASS()
class PAWSINTHESHELL_API APITS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APITS_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

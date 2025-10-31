// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PawsInTheShell/Public/PITS_Character.h"

// Sets default values
APITS_Character::APITS_Character()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APITS_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void APITS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


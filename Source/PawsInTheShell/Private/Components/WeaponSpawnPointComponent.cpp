// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Components/WeaponSpawnPointComponent.h"

UWeaponSpawnPointComponent::UWeaponSpawnPointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UWeaponSpawnPointComponent::BeginPlay()
{
	Super::BeginPlay();
}

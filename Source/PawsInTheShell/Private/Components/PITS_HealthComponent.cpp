// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Components/PITS_HealthComponent.h"

UPITS_HealthComponent::UPITS_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UPITS_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPITS_HealthComponent::AddHealth(const float HealthToAdd)
{
	if (IsDead()) return;

	CurrentHealth = FMath::Clamp(CurrentHealth + HealthToAdd, 0.0f, MaxHealth);
	if (IsDead())
	{
		OnZeroHealth.Broadcast();
	}
}

void UPITS_HealthComponent::RemoveHealth(const float HealthToRemove)
{
	AddHealth(HealthToRemove * -1.0f);
}


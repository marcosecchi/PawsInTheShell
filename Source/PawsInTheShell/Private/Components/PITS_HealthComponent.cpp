// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Components/PITS_HealthComponent.h"

#include "Utils/PITS_Logs.h"

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
	UE_LOG(LogPITS, Log, TEXT("'%s' Health changed by %f. Current Health: %f/%f"), *GetNameSafe(this), HealthToAdd, CurrentHealth, MaxHealth);
	if (IsDead()) OnZeroHealth.Broadcast();
	if (GetHealthPercentage() == 1.0f) OnFullHealth.Broadcast();
	OnUpdateHealth.Broadcast(GetHealthPercentage());
}

void UPITS_HealthComponent::RemoveHealth(const float HealthToRemove)
{
	AddHealth(HealthToRemove * -1.0f);
}


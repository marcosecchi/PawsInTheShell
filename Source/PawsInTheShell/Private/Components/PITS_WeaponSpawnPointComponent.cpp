// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Components/PITS_WeaponSpawnPointComponent.h"

#include "PITS_BaseProjectile.h"

UPITS_WeaponSpawnPointComponent::UPITS_WeaponSpawnPointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UPITS_WeaponSpawnPointComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPITS_WeaponSpawnPointComponent::Shoot()
{
	if (CurrentProjectileClass)
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			const FVector SpawnLocation = GetComponentLocation();
			const FRotator SpawnRotation = GetComponentRotation();
			World->SpawnActor<APITS_BaseProjectile>(CurrentProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		}
	}
}

void UPITS_WeaponSpawnPointComponent::SetCurrentProjectileClass(
	const TSubclassOf<APITS_BaseProjectile> NewProjectileClass)
{
	CurrentProjectileClass = NewProjectileClass;
}

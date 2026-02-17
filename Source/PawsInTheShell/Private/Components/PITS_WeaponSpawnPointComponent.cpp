// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Components/PITS_WeaponSpawnPointComponent.h"
#include "PITS_BaseProjectile.h"
#include "Subsystems/PITS_ObjectPoolSubsystem.h"

UPITS_WeaponSpawnPointComponent::UPITS_WeaponSpawnPointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPITS_WeaponSpawnPointComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PoolSubsystem = GetWorld()->GetSubsystem<UPITS_ObjectPoolSubsystem>();
}

void UPITS_WeaponSpawnPointComponent::Shoot()
{
	if (CurrentProjectileClass)
	{
		const FVector SpawnLocation = GetComponentLocation();
		const FRotator SpawnRotation = GetComponentRotation();
		const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
		PoolSubsystem->AcquirePooledObject(CurrentProjectileClass, SpawnTransform);
	}
}

void UPITS_WeaponSpawnPointComponent::SetCurrentProjectileClass(
	const TSubclassOf<APITS_BaseProjectile> NewProjectileClass)
{
	CurrentProjectileClass = NewProjectileClass;
	if (PoolSubsystem)
	{
		PoolSubsystem->CreateObjectPool(CurrentProjectileClass, RequestedPoolSize);
	}
}

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
	
	WorldSubsystem = GetWorld()->GetSubsystem<UPITS_WorldSubsystem>();
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
			const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
			WorldSubsystem->AcquirePooledObject(CurrentProjectileClass, SpawnTransform);
		}
	}
}

void UPITS_WeaponSpawnPointComponent::SetCurrentProjectileClass(
	const TSubclassOf<APITS_BaseProjectile> NewProjectileClass)
{
	CurrentProjectileClass = NewProjectileClass;
	if (WorldSubsystem)
	{
		WorldSubsystem->CreateObjectPool(CurrentProjectileClass, RequestedPoolSize);
	}
}

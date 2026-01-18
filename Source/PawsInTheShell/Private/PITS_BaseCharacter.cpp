// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_BaseCharacter.h"

#include "Components/PITS_ArmourComponent.h"
#include "Subsystems/PITS_WorldSubsystem.h"
#include "Damage/PITS_DamageType_Regeneration.h"
#include "Components/PITS_HealthComponent.h"
#include "Components/PITS_WeaponSpawnPointComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Structs/PITS_CharacterDataTableRow.h"
#include "Structs/PITS_WeaponDataTableRow.h"
#include "Utils/PITS_Logs.h"

// Sets default values
APITS_BaseCharacter::APITS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Health = CreateDefaultSubobject<UPITS_HealthComponent>(FName("Health"));
	Armour = CreateDefaultSubobject<UPITS_ArmourComponent>(FName("Armour"));
}

void APITS_BaseCharacter::HandleShoot()
{
	TArray<UPITS_WeaponSpawnPointComponent*> WeaponSpawnPoints;
	GetComponents<UPITS_WeaponSpawnPointComponent>(WeaponSpawnPoints);
	if (ShootDelay > 0.f && (GetVelocity().Size() <= KINDA_SMALL_NUMBER))
	{
		FTimerHandle ShootDelayTimerHandle;
		GetWorldTimerManager().SetTimer(ShootDelayTimerHandle, [this, WeaponSpawnPoints]()
		{
			for (UPITS_WeaponSpawnPointComponent* SpawnPoint : WeaponSpawnPoints)
			{
				SpawnPoint->Shoot();
			}
		}, ShootDelay, false);
		return;
	}
	for (UPITS_WeaponSpawnPointComponent* SpawnPoint : WeaponSpawnPoints)
	{
		SpawnPoint->Shoot();
	}
}

void APITS_BaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (const FPITS_CharacterDataTableRow* CharacterData = CharacterStatsType.GetRow<FPITS_CharacterDataTableRow>(FString()))
	{
		GetCharacterMovement()->JumpZVelocity = CharacterData->JumpZVelocity;
		GetCharacterMovement()->AirControl = CharacterData->AirControl;
		GetCharacterMovement()->MaxWalkSpeed = CharacterData->MaxWalkSpeed;
		GetCharacterMovement()->BrakingDecelerationWalking = CharacterData->BrakingDecelerationWalking;
		GetCharacterMovement()->BrakingDecelerationFalling = CharacterData->BrakingDecelerationFalling;

		Health->SetMaxHealth(CharacterData->MaxHealth);
		Health->SetCurrentHealth(CharacterData->MaxHealth);
		Health->SetCanRegenerate(CharacterData->bCanRegenerate);
		
		Armour->SetArmourAmount(CharacterData->ArmourAmount);
		Armour->SetIsCybernetic(CharacterData->bCyberAugmented);

		bIsCybernetic = CharacterData->bCyberAugmented;
		CharacterName = CharacterData->CharacterName;
		CharacterDescription = CharacterData->CharacterDescription;
		CharacterIcon = CharacterData->CharacterIcon;
	}

	if (const FPITS_WeaponDataTableRow* WeaponData = WeaponStatsType.GetRow<FPITS_WeaponDataTableRow>(FString()))
	{
		WeaponName = WeaponData->WeaponName;
		WeaponCooldownTime = WeaponData->CooldownBetweenShots;
		TArray<UPITS_WeaponSpawnPointComponent*> WeaponSpawnPoints;
		GetComponents<UPITS_WeaponSpawnPointComponent>(WeaponSpawnPoints);
		for (UPITS_WeaponSpawnPointComponent* SpawnPoint : WeaponSpawnPoints)
		{
			SpawnPoint->SetCurrentProjectileClass(WeaponData->ProjectileClass);
		}
	}
}

float APITS_BaseCharacter::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent,
                                      AController* EventInstigator, AActor* DamageCauser)
{
	float EffectiveAmount = DamageAmount;
	
	UClass* DamageTypeClass = UDamageType::StaticClass();
	if (DamageEvent.DamageTypeClass != nullptr)
	{
		DamageTypeClass = DamageEvent.DamageTypeClass;
	}
	
	// Check if damage type is regeneration
	if (DamageTypeClass->IsChildOf(UPITS_DamageType_Regeneration::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("Regeneration damage type detected, healing character."));
		Health->AddHealth(EffectiveAmount);
	}
	else
	{
		// Apply armour calculations
		EffectiveAmount = Armour->GetActualDamage(DamageAmount, DamageTypeClass);
		UE_LOG(LogPITS, Log, TEXT("Taking damage: Original Amount = %f, Effective Amount after Armour = %f"), DamageAmount, EffectiveAmount);
		Health->RemoveHealth(EffectiveAmount);
	}

	if (const UPITS_WorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UPITS_WorldSubsystem>())
	{
		WorldSubsystem->NotifyDamageTaken(DamageTypeClass, EffectiveAmount);
	}
	return EffectiveAmount;
}

void APITS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Find all weapon components set their projectile class from the data table
	TArray<UPITS_WeaponSpawnPointComponent*> WeaponSpawnPoints;
	GetComponents<UPITS_WeaponSpawnPointComponent>(WeaponSpawnPoints);
	if (const FPITS_WeaponDataTableRow* WeaponData = WeaponStatsType.GetRow<FPITS_WeaponDataTableRow>(FString()))
	{
		for (UPITS_WeaponSpawnPointComponent* SpawnPoint : WeaponSpawnPoints)
		{
			SpawnPoint->SetCurrentProjectileClass(WeaponData->ProjectileClass);
		}
	}
	Health->OnUpdateHealth.BindLambda([this](const EPITS_HealthStatus Status)
	{
		if (Status != EPITS_HealthStatus::Death)
		{
			OnUpdateHealthStatus.Broadcast(Health->GetHealthPercentage());
		}
		// TODO: Implement death logic
		HandleUpdateHealthStatus(Health->GetHealthPercentage());
	});
	// Initial health update
	HandleUpdateHealthStatus(Health->GetHealthPercentage());
}

#pragma region HealthInterface Implementations
bool APITS_BaseCharacter::IsDead_Implementation() const
{
	return Health->IsDead();
}

float APITS_BaseCharacter::GetHealthPercentage_Implementation() const
{
	return Health->GetHealthPercentage();
}

bool APITS_BaseCharacter::CanRegenerate_Implementation() const
{
	return Health->CanRegenerate() && !IsDead_Implementation() && Health->GetHealthPercentage() > 0.0f;
}
#pragma endregion

#pragma region DefenceInterface Implementations
float APITS_BaseCharacter::GetArmourAmount_Implementation() const
{
	return Armour->GetArmourAmount();
}

bool APITS_BaseCharacter::IsCybernetic_Implementation() const
{
	return bIsCybernetic;
}
#pragma endregion


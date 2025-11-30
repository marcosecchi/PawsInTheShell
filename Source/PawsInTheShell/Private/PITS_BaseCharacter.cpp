// Fill out your copyright notice in the Description page of Project Settings.// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_BaseCharacter.h"

#include "Subsystems/PITS_WorldSubsystem.h"
#include "Damage/PITS_DamageType_Regeneration.h"
#include "Components/PITS_HealthComponent.h"
#include "Components/PITS_WeaponSpawnPointComponent.h"
#include "Damage/PITS_DamageType_CyberTech.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Structs/PITS_CharacterDataTableRow.h"
#include "Structs/PITS_WeaponDataTableRow.h"

// Sets default values
APITS_BaseCharacter::APITS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Health = CreateDefaultSubobject<UPITS_HealthComponent>(FName("Health"));
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
		Health->SetCurrentHealth(CharacterData->StartingHealth);
		Health->SetCanRegenerate(CharacterData->bCanRegenerate);
		
		ArmourAmount = CharacterData->ArmourAmount;
		bIsCybernetic = CharacterData->bCyberAugmented;
		CharacterName = CharacterData->CharacterName;
		CharacterDescription = CharacterData->CharacterDescription;
		CharacterIcon = CharacterData->CharacterIcon;
	}

	if (const FPITS_WeaponDataTableRow* WeaponData = WeaponStatsType.GetRow<FPITS_WeaponDataTableRow>(FString()))
	{
		WeaponName = WeaponData->WeaponName;
	}
}

float APITS_BaseCharacter::TakeDamage(const float DamageAmount, struct FDamageEvent const& DamageEvent,
                                      AController* EventInstigator, AActor* DamageCauser)
{
	UPITS_WorldSubsystem *WorldSubsystem = GetWorld()->GetSubsystem<UPITS_WorldSubsystem>();

	float EffectiveAmount = DamageAmount;
	
	if (DamageEvent.DamageTypeClass != nullptr)
	{
		// Check if damage type is regeneration
		if (DamageEvent.DamageTypeClass->IsChildOf(UPITS_DamageType_Regeneration::StaticClass()))
		{
			Health->AddHealth(EffectiveAmount);
		}
		// Check if damage type is cybertech
		else if (DamageEvent.DamageTypeClass->IsChildOf(UPITS_DamageType_CyberTech::StaticClass()))
		{
			if (!bIsCybernetic)
			{
				// Non-cybernetic characters are unaffected by CyberTech damage
				return 0.0f;
			}
			// PicoTech damage ignores armor for cybernetic characters
			Health->RemoveHealth(EffectiveAmount);
		}
		else
		{
			// Apply armor reduction
			EffectiveAmount = FMath::Max(0.0f, DamageAmount - ArmourAmount);
			Health->RemoveHealth(EffectiveAmount);
		}
	}
	if (WorldSubsystem)
	{
		WorldSubsystem->NotifyDamageTaken(DamageEvent.DamageTypeClass, EffectiveAmount);
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
	
	Health->OnFullHealth.AddDynamic(this, &APITS_BaseCharacter::HandleFullHealth);
	Health->OnZeroHealth.AddDynamic(this, &APITS_BaseCharacter::HandleZeroHealth);
	Health->OnUpdateHealth.AddDynamic(this, &APITS_BaseCharacter::HandleUpdateHealth);
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
	return ArmourAmount;
}

bool APITS_BaseCharacter::IsCybernetic_Implementation() const
{
	return bIsCybernetic;
}
#pragma endregion

#pragma region Delegates
void APITS_BaseCharacter::HandleFullHealth()
{
	OnUpdateHealth.Broadcast(Health->GetHealthPercentage());
}

void APITS_BaseCharacter::HandleZeroHealth()
{
	OnUpdateHealth.Broadcast(Health->GetHealthPercentage());
}

void APITS_BaseCharacter::HandleUpdateHealth(float HealthPercentage)
{
	OnUpdateHealth.Broadcast(Health->GetHealthPercentage());
}
#pragma endregion 


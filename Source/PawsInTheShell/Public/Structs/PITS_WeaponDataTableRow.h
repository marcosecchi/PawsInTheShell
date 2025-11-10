// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PITS_WeaponDataTableRow.generated.h"

class APITS_BaseProjectile;

/**
 *  Holds information about weapon stats and properties
 */
USTRUCT(BlueprintType, meta=(DisplayName="WeaponDataTableRow"))
struct FPITS_WeaponDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(ToolTip="Name of the weapon displayed in game"))
	FText WeaponName;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Maximum number of ammunitions reachable by the weapon. Set To -1 for infinite ammo"))
	float MaxAmmo = 100.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Projectile that will be spawned by this weapon"))
	TSubclassOf<APITS_BaseProjectile> ProjectileClass = nullptr;

	UPROPERTY(EditAnywhere, meta=(ToolTip="A multiplier applied to the projectile noise value when fired from this weapon"))
	float NoiseMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Cooldown time between consecutive shots fired by this weapon"))
	float CooldownBetweenShots = 0.0f;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Additional notes about this weapon"))
	FString Notes;
};

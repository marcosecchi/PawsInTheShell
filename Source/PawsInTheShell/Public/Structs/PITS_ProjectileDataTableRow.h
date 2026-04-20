// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Object.h"
#include "PITS_ProjectileDataTableRow.generated.h"

class UNiagaraSystem;

/**
 * Holds information about projectile stats and properties
 */
USTRUCT(BlueprintType, meta=(DisplayName="ProjectileDataTableRow"))
struct FPITS_ProjectileDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Name of the bullet displayed in game"))
	FText ProjectileName = FText::FromString("DefaultProjectile");
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Mesh used to represent this projectile in game"))
	UStaticMesh* ProjectileMesh = nullptr;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Damage dealt by this projectile upon impact"))
	float Damage = 100.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Speed of the projectile when fired"))
	float Speed = 3000.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 50000),
		meta=(ToolTip="Physics force to apply on hit"))
	float PhysicsForce = 100.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Type of damage provoked by the bullet"))
	TSubclassOf<UDamageType> DamageType = nullptr;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="VFX to spawn when the bullet hits something"))
	TObjectPtr<UNiagaraSystem> VfxHit;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Additional notes about this bullet"))
	FString Notes;
};

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "PITS_DamageType_ArmourPiercing.generated.h"

/**
 * Damage type used by Armour Piercing weapons that will ignore part of the target's armour
 */
UCLASS(meta=(DisplayName="DamageType_ArmourPiercing"))
class PAWSINTHESHELL_API UPITS_DamageType_ArmourPiercing : public UDamageType
{
	GENERATED_BODY()
};

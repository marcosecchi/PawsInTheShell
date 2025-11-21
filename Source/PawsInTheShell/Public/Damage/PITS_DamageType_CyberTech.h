// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "PITS_DamageType_CyberTech.generated.h"

/**
 * Damage type used by PicoTech weapons that will affect cybernetic characters
 */
UCLASS(meta=(DisplayName="DamageType_CyberTech"))
class PAWSINTHESHELL_API UPITS_DamageType_CyberTech : public UDamageType
{
	GENERATED_BODY()
};

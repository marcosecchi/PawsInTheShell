// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "PITS_DamageType_Regeneration.generated.h"

/**
 *  Damage type used for regeneration zones
 */
UCLASS(meta=(DisplayName="DamageType_Regeneration"))
class PAWSINTHESHELL_API UPITS_DamageType_Regeneration : public UDamageType
{
	GENERATED_BODY()
};

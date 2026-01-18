// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Components/PITS_ArmourComponent.h"

#include "Damage/PITS_DamageType_ArmourPiercing.h"
#include "Damage/PITS_DamageType_CyberTech.h"
#include "Engine/DamageEvents.h"

UPITS_ArmourComponent::UPITS_ArmourComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UPITS_ArmourComponent::GetActualDamage(const float IncomingDamage, const UClass* DamageTypeClass) const
{
	if (DamageTypeClass->IsChildOf(UPITS_DamageType_CyberTech::StaticClass()))
	{
		if (!bIsCybernetic)
		{
			// Non-cybernetic characters are unaffected by CyberTech damage
			return 0.0f;
		}
		// PicoTech damage ignores armor for cybernetic characters
		return IncomingDamage;
	}
	if (DamageTypeClass->IsChildOf(UPITS_DamageType_ArmourPiercing::StaticClass()))
	{
		// Armour Piercing damage ignores armour
		return IncomingDamage;
	}
	// Apply armor reduction
	return FMath::Max(0.0f, IncomingDamage - ArmourAmount);
}

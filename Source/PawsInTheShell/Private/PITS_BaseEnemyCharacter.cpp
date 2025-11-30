// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_BaseEnemyCharacter.h"
#include "Utils/PITS_Globals.h"

APITS_BaseEnemyCharacter::APITS_BaseEnemyCharacter()
{
	// Assign the enemy tag
	Tags.Add(UPITS_Globals::GetEnemyTag());
}


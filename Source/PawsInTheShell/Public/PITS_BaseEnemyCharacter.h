// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BaseCharacter.h"
#include "PITS_BaseEnemyCharacter.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class PAWSINTHESHELL_API APITS_BaseEnemyCharacter : public APITS_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APITS_BaseEnemyCharacter();
};

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BaseCharacter.h"
#include "GameFramework/Character.h"
#include "PITS_BaseNPCCharacter.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class PAWSINTHESHELL_API APITS_BaseNPCCharacter : public APITS_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APITS_BaseNPCCharacter();

protected:

public:
};

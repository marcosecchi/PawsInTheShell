// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PITS_ShooterInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType, Blueprintable, Category="Paws In The Shell")
class UPITS_ShooterInterface : public UInterface
{
	GENERATED_BODY()
};

class IPITS_ShooterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Weapon")
	void Shoot();
};
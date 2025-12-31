// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PITS_AmmoInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable, Category="Paws In The Shell")
class UPITS_AmmoInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Common interface for actor defence mechanisms
 */
class PAWSINTHESHELL_API IPITS_AmmoInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Ammo")
	int GetCurrentAmmoAmount() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Ammo")
	int GetMaxAmmoAmount() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Ammo")
	int AddAmmo(const int Amount);
};
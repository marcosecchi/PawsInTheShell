// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterDefenceInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable, MinimalAPI, meta=(RestrictedToClasses="ACharacter"))
class UCharacterDefenceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Common interface for actor defence mechanisms
 */
class PAWSINTHESHELL_API ICharacterDefenceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Defence")
	bool IsInSafeZone() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Defence")
	float GetArmourAmount() const;

};
// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DefenceInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable, MinimalAPI, meta=(RestrictedToClasses="AActor"))
class UDefenceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Common interface for actor defence mechanisms
 */
class PAWSINTHESHELL_API IDefenceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Defence")
	void SetIsInSafeZone(const bool bNewInSafeZone);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Defence")
	bool IsInSafeZone() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Defence")
	float GetArmourAmount() const;

};
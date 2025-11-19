// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PITS_HealthInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable)
class UPITS_HealthInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Common interface for actor health status
 */
class PAWSINTHESHELL_API IPITS_HealthInterface
{
	GENERATED_BODY()

public:
	/** Returns true if the actor is dead */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Health")
	bool IsDead() const;
	
	/** Returns the current health percentage (0 to 1) of the actor */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Health")
	float GetHealthPercentage() const;

	/** Returns true if the actor can regenerate health */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Health")
	bool CanRegenerate();
};
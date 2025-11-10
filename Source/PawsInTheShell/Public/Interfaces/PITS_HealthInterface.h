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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Health")
	bool IsDead() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Health")
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Health")
	bool CanRegenerate();
};
// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthInterface.generated.h"

UINTERFACE(BlueprintType, MinimalAPI)
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Common interface for actor health status
 */
class PAWSINTHESHELL_API IHealthInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Health")
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Health")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Health")
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Health")
	bool CanRegenerate();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Health")
	void StartRegenerating();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Health")
	void StopRegenerating();
};
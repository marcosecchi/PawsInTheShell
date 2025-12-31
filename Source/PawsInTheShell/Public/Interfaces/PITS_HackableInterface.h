// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PITS_HackableInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType, Blueprintable, Category="PawsInTheShell")
class UPITS_HackableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Common interface for hackable actors
 */
class PAWSINTHESHELL_API IPITS_HackableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Hack")
	void Hack();
};
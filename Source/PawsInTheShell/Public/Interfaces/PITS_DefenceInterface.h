// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PITS_DefenceInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable, Category="Paws In The Shell")
class UPITS_DefenceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Common interface for actor defence mechanisms
 */
class PAWSINTHESHELL_API IPITS_DefenceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Defence")
	float GetArmourAmount() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Defence")
	bool IsCybernetic() const;

};
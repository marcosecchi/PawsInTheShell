// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PITS_PooledObjectInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType, Blueprintable, Category="Paws In The Shell")
class UPITS_PooledObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class PAWSINTHESHELL_API IPITS_PooledObjectInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Object Pool")
	void HandleAcquire();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Object Pool")
	void HandleRelease();
};


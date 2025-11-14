// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PITS_StatsWidgetInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType, Blueprintable)
class UPITS_StatsWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class IPITS_StatsWidgetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|UI")
	void UpdateName(const FText &NewName);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|UI")
	void UpdateHealth(const float Value);
};


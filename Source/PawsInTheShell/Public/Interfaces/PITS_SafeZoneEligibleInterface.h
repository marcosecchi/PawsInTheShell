// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"

#include "PITS_SafeZoneEligibleInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable)
class UPITS_SafeZoneEligibleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Common interface for player character safe zones mechanisms
 */
class PAWSINTHESHELL_API IPITS_SafeZoneEligibleInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Defence")
	void SetIsInSafeZone(const bool bNewInSafeZone);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|Defence")
	bool IsInSafeZone() const;
};
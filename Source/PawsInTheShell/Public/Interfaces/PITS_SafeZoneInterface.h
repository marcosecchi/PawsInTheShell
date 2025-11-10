// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PITS_SafeZoneInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable)
class UPITS_SafeZoneInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Common interface for player character defence mechanisms
 */
class PAWSINTHESHELL_API IPITS_SafeZoneInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Defence")
	void SetIsInSafeZone(const bool bNewInSafeZone);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character|Defence")
	bool IsInSafeZone() const;
};
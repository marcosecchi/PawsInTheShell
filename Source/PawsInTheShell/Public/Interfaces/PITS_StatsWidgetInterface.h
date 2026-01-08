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
UINTERFACE(BlueprintType, Blueprintable, Category="PawsInTheShell")
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
	void UpdateDescription(const FText &NewDescription);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|UI")
	void UpdateIcon(const UTexture2D* NewIcon);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|UI")
	void UpdateHealth(const float Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PawsInTheShell|UI")
	void UpdateWeaponData(const FText &WeaponName, const int32 CurrentAmmo, const int32 MaxAmmo);
};


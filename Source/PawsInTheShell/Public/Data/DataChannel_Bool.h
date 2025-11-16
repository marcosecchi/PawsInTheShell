// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataChannel_Bool.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FValueChangeDelegate, bool, NewValue);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Category="PawsInTheShell|DataChannels")
class PAWSINTHESHELL_API UDataChannel_Bool : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool Value;
	
public:
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|DataChannels")
	void SetValue(const bool NewValue);

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|DataChannels")
	bool GetValue() const { return Value; }
	
	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|DataChannels")
	FValueChangeDelegate OnChangeValue;

};

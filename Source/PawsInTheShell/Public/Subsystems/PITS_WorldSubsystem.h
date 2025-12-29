// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PITS_WorldSubsystem.generated.h"

class UPITS_FixedActorPool;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTaken, TSubclassOf<UDamageType>, DamageEvent, float, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageEnd, TSubclassOf<UDamageType>, DamageEvent);

/**
 * 
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_WorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void ChangeCharacter();
	
#pragma region "Damage"
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void NotifyDamageTaken(const TSubclassOf<UDamageType>& DamageType, const float Amount);

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void NotifyDamageEnd(const TSubclassOf<UDamageType>& DamageType);

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Subsystems")
	FOnDamageTaken OnDamageTaken;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Subsystems")
	FOnDamageEnd OnDamageEnd;
#pragma endregion
	

};

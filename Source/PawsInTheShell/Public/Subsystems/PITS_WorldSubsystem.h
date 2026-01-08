// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PITS_WorldSubsystem.generated.h"

class UPITS_FixedActorPool;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDamageTaken, TSubclassOf<UDamageType>, DamageEvent, float, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDamageEnd, TSubclassOf<UDamageType>, DamageEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterUpdateWeapon);

/**
 * 
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_WorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void ChangeCharacter() const;
	
#pragma region "Damage"
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void NotifyDamageTaken(const TSubclassOf<UDamageType>& DamageType, const float Amount) const;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void NotifyDamageEnd(const TSubclassOf<UDamageType>& DamageType) const;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void NotifyUpdateWeapon() const;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Subsystems")
	FOnCharacterDamageTaken OnCharacterDamageTaken;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Subsystems")
	FOnCharacterDamageEnd OnCharacterDamageEnd;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Subsystems")
	FOnCharacterUpdateWeapon OnCharacterUpdateWeapon;
#pragma endregion
	

};

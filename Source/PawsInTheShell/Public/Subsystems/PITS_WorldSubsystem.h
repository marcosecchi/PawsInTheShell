// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PITS_WorldSubsystem.generated.h"

class ATriggerVolume;
class UPITS_FixedActorPool;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCharacterDamageTakenSignature, TSubclassOf<UDamageType>, DamageEvent, float, Amount, APawn*, DamagedPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDamageEndSignature, TSubclassOf<UDamageType>, DamageEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterUpdateWeaponSignature);

/**
 *  World subsystem that manages global events related to the player character
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
	void NotifyDamageTaken(const TSubclassOf<UDamageType>& DamageType, const float Amount, APawn* DamagedPawn) const;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void NotifyDamageEnd(const TSubclassOf<UDamageType>& DamageType) const;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void NotifyUpdateWeapon() const;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Subsystems")
	FOnCharacterDamageTakenSignature OnCharacterDamageTaken;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Subsystems")
	FOnCharacterDamageEndSignature OnCharacterDamageEnd;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Subsystems")
	FOnCharacterUpdateWeaponSignature OnCharacterUpdateWeapon;
#pragma endregion
	
#pragma region "Spawners"
	
protected:
	
	static void SpawnActorsOnGameThread(UWorld* World, const TSubclassOf<AActor> SpawnableActorClass, TArray<FVector> SpawnLocations);
	
public:
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void SpawnActorsAsync(TSubclassOf<AActor> SpawnableActorClass, const int32 MinAmount, const int32 MaxAmount, ATriggerVolume* SpawnArea);

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Subsystems")
	void SpawnActorsOnNavMesh(TSubclassOf<AActor> SpawnableActorClass, const int32 MinAmount, const int32 MaxAmount, const FVector SpawnCenterLocation, const float SpawnRadius);
#pragma endregion


};

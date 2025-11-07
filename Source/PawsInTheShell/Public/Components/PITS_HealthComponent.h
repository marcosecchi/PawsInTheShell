// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PITS_HealthComponent.generated.h"

// Declare dead delegate for health component
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPITS_ZeroHealthDelegate);

UCLASS(Blueprintable, BlueprintType, ClassGroup=("Paws In The Shell"), meta=(BlueprintSpawnableComponent))
class PAWSINTHESHELL_API UPITS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPITS_HealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	float CurrentHealth = 1.0f;

	UPROPERTY()
	float MaxHealth = 1.0f;

	UPROPERTY()
	bool bCanRegenerate = false;

public:

	UFUNCTION()
	void SetCurrentHealth(const float NewCurrentHealth) { CurrentHealth = FMath::Clamp(NewCurrentHealth, 0.0f, MaxHealth); }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetCurrentHealth() const { return CurrentHealth; }
	
	UFUNCTION()
	void SetMaxHealth(const float NewMaxHealth) { MaxHealth = FMath::Max(0.0f, NewMaxHealth); }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const { return MaxHealth; }
	
	UFUNCTION()
	void SetCanRegenerate(const bool bNewCanRegenerate) {  bCanRegenerate = bNewCanRegenerate; }

	UFUNCTION(BlueprintCallable, Category="Health")
	bool CanRegenerate() const { return bCanRegenerate; }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealthPercentage() const { return (MaxHealth > 0.0f) ? (CurrentHealth / MaxHealth) : 0.0f; }

	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsDead() const { return CurrentHealth <= 0.0f; }

	UFUNCTION(BlueprintCallable, Category="Health")
	void AddHealth(const float HealthToAdd);

	UFUNCTION(BlueprintCallable, Category="Health")
	void RemoveHealth(const float HealthToRemove);	
	
#pragma region Delegate Broadcasts
	UPROPERTY(BlueprintAssignable, Category="Health")
	FPITS_ZeroHealthDelegate OnZeroHealth;

#pragma endregion

};

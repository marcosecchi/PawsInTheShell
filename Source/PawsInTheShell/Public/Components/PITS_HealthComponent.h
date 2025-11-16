// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PITS_HealthComponent.generated.h"

// Declare delegates for health component
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZeroHealthDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFullHealthDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthUpdateDelegate, float, HealthPercentage);

UCLASS(Blueprintable, BlueprintType, ClassGroup=("PawsInTheShell"), meta=(BlueprintSpawnableComponent))
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

	UFUNCTION()
	void SetMaxHealth(const float NewMaxHealth) { MaxHealth = FMath::Max(0.0f, NewMaxHealth); }
	
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
public:
	UPROPERTY(BlueprintAssignable, Category="Health")
	FZeroHealthDelegate OnZeroHealth;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FFullHealthDelegate OnFullHealth;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FHealthUpdateDelegate OnUpdateHealth;

#pragma endregion

};

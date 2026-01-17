// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PITS_HealthComponent.generated.h"

UENUM(BlueprintType)
enum class EPITS_HealthStatus : uint8
{
	Update		UMETA(DisplayName="Update"),
	Maximum		UMETA(DisplayName="Maximum"),
	Death		UMETA(DisplayName="Death"),
};

// Declare delegate for health component
DECLARE_DELEGATE_OneParam(FHealthUpdateSignature, EPITS_HealthStatus /* HealthStatus */);

UCLASS(BlueprintType, DisplayName="Health", ClassGroup=("PawsInTheShell"))
class PAWSINTHESHELL_API UPITS_HealthComponent final : public UActorComponent
{
	GENERATED_BODY()

public:
	UPITS_HealthComponent();

protected:
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
	
#pragma region "Delegates"
public:
	FHealthUpdateSignature OnUpdateHealth;
#pragma endregion

};

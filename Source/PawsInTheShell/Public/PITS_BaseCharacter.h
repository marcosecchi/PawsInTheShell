// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/PITS_DefenceInterface.h"
#include "Interfaces/PITS_HealthInterface.h"
#include "PITS_BaseCharacter.generated.h"

class UPITS_HealthComponent;
class UPITS_ArmourComponent;
struct FDamageEvent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterHealthStatusUpdateSignature, float, HealthPercentage);
// TODO: Declare death delegate

/**
 * Base character class
 */
UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class PAWSINTHESHELL_API APITS_BaseCharacter : public ACharacter, public IPITS_HealthInterface, public IPITS_DefenceInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPITS_HealthComponent> Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPITS_ArmourComponent> Armour;

public:
	APITS_BaseCharacter();

	/** Called when the character takes damage */
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Delegate called when the health status is updated */
	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Health")
	FCharacterHealthStatusUpdateSignature OnUpdateHealthStatus;

protected:
	virtual void BeginPlay() override;
	
	/** Data on the type of picked weapon and visuals of this pickup */
	UPROPERTY(EditDefaultsOnly, Category="PawsInTheShell|Stats")
	FDataTableRowHandle CharacterStatsType;

	/** Data on the type of picked weapon and visuals of this pickup */
	UPROPERTY(EditDefaultsOnly, Category="PawsInTheShell|Stats")
	FDataTableRowHandle WeaponStatsType;
	
	UPROPERTY(BlueprintReadOnly, Category="PawsInTheShell|Stats")
	FText CharacterName = FText::FromString("Unnamed");

	UPROPERTY(BlueprintReadOnly, Category="PawsInTheShell|Stats")
	FText CharacterDescription = FText::FromString("");

	UPROPERTY(BlueprintReadOnly, Category="PawsInTheShell|Stats")
	UTexture2D* CharacterIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="PawsInTheShell|Stats")
	FText WeaponName = FText::FromString("Unnamed");

	UPROPERTY(BlueprintReadOnly, Category="PawsInTheShell|Stats")
	float WeaponCooldownTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="PawsInTheShell|Weapon")
	float ShootDelay = 0.0f;
	
	UPROPERTY()
	bool bIsCybernetic = false;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Weapon")
	void HandleShoot();

	UFUNCTION(BlueprintImplementableEvent, Category="PawsInTheShell|Weapon")
	void HandleUpdateHealthStatus(const float HealthPercentage);

	/** Native construction script */
	virtual void OnConstruction(const FTransform& Transform) override;

#pragma region Property Getters and Setters
public:
	/* Returns the character name from the data table */
	FORCEINLINE FText GetCharacterName() const { return CharacterName; }

	/* Returns the character name from the data table */
	FORCEINLINE FText GetCharacterDescription() const { return CharacterDescription; }

	/* Returns the character name from the data table */
	FORCEINLINE UTexture2D* GetCharacterIcon() const { return CharacterIcon; }

	/* Returns the weapon name from the data table */
	FORCEINLINE FText GetWeaponName() const { return WeaponName; }

	/* Returns the weapon cooldown time from the data table */
	FORCEINLINE float GetWeaponCooldownTime() const { return WeaponCooldownTime; }
	
#pragma endregion

#pragma region HealthInterface Implementations
public:
	virtual bool IsDead_Implementation() const override;
	virtual float GetHealthPercentage_Implementation() const override;
	virtual bool CanRegenerate_Implementation() const override;
#pragma endregion

#pragma region DefenceInterface Implementations
public:
	virtual float GetArmourAmount_Implementation() const override;
	
	virtual bool IsCybernetic_Implementation() const override;
#pragma endregion
};

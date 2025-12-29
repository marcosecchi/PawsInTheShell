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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterHealthUpdateDelegate, float, HealthPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDeathDelegate, AActor*, DeadCharacter);

UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class PAWSINTHESHELL_API APITS_BaseCharacter : public ACharacter, public IPITS_HealthInterface, public IPITS_DefenceInterface
{
	GENERATED_BODY()

	/** Health */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPITS_HealthComponent> Health;

public:
	// Sets default values for this character's properties
	APITS_BaseCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Health")
	FCharacterHealthUpdateDelegate OnUpdateHealth;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|Health")
	FCharacterDeathDelegate OnDeath;

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

	UPROPERTY()
	float ArmourAmount = 0.0f;

	UPROPERTY()
	bool bIsCybernetic = false;

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Weapon")
	void HandleShoot();
	
	/** Native construction script */
	virtual void OnConstruction(const FTransform& Transform) override;

#pragma region Delegates
	UFUNCTION()
	void HandleFullHealth();
	
	UFUNCTION()
	void HandleZeroHealth();
	
	UFUNCTION()
	void HandleUpdateHealth(float HealthPercentage);
#pragma endregion
	
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

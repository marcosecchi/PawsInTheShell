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

/**
 *  Holds information about character stats and properties
 */
USTRUCT(BlueprintType)
struct FCharacterDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(ToolTip="Name of the character displayed in game"))
	FText CharacterName;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Maximum health reachable by the character"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Character health at the start of the game"))
	float StartingHealth = 100.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Character defensive amount that reduces incoming damage"))
	float ArmourAmount = 0.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Can the character regenerate when in a regeneration zone?"))
	bool bCanRegenerate = false;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Maximum speed of the character when moving"))
	float MaxWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Initial velocity (instantaneous force acceleration) applied when the character jumps"))
	float JumpZVelocity = 500.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Amount of lateral control for the character while in air (0 = no control, 1 = full control (Max Walk Speed)"))
	float AirControl = 0.35f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Deceleration applied when walking and not applying acceleration"))
	float BrakingDecelerationWalking = 2000.f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="LAteral deceleration applied when falling and not applying acceleration"))
	float BrakingDecelerationFalling = 1500.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Additional notes about this character (for development purposes)"))
	FString Notes;

};

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

protected:
	/** Data on the type of picked weapon and visuals of this pickup */
	UPROPERTY(EditDefaultsOnly, Category="Character|Stats")
	FDataTableRowHandle CharacterStatsType;
	
	UPROPERTY()
	FText CharacterName = FText::FromString("Unnamed");

	UPROPERTY()
	float ArmourAmount = 0.0f;
	
	/** Native construction script */
	virtual void OnConstruction(const FTransform& Transform) override;

#pragma region Property Getters and Setters
public:
	/* Returns the character name from the data table */
	FORCEINLINE FText GetCharacterName() const { return CharacterName; }

#pragma endregion

#pragma region HealthInterface Implementations
public:
	virtual bool IsDead_Implementation() const override;
	virtual float GetHealthPercentage_Implementation() const override;
	virtual bool CanRegenerate_Implementation() override;
#pragma endregion

#pragma region DefenceInterface Implementations
public:
	virtual float GetArmourAmount_Implementation() const override;
	
#pragma endregion

};

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/PITS_DefenceInterface.h"
#include "Interfaces/PITS_HealthInterface.h"
#include "PITS_BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UPITS_HealthComponent;
struct FInputActionValue;

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

UCLASS(Abstract)
class PAWSINTHESHELL_API APITS_BaseCharacter : public ACharacter, public IPITS_HealthInterface, public IPITS_DefenceInterface
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Camera boom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Health */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPITS_HealthComponent> Health;

public:
	APITS_BaseCharacter();

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
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Actions
	/** Input Action: Jump */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Input Action: Move */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Input Action: Look */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* LookAction;

	/** Input Action: Mouse Look */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MouseLookAction;

	/** Input Action: Shoot */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ShootAction;

	/** Input Action: Change Character */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ChangeCharacterAction;
#pragma endregion

	UPROPERTY()
	bool bInSafeZone = false;
	
	/** Called from Input Actions for movement input */
	void Move(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void Look(const FInputActionValue& Value);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void JumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void JumpEnd();

	/** Handles shoot inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void Shoot();

	/** Handles change character inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void ChangeCharacter();

#pragma region Property Getters and Setters
public:
	
	/** Returns the camera component **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns the camera boom component **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/* Returns the character name from the data table */
	FORCEINLINE FText GetCharacterName() const { return CharacterName; }
	
	/* Sets if the character has reached a safe zone */
	UFUNCTION(BlueprintCallable, Category="Character")
	FORCEINLINE void SetInSafeZone(const bool bNewInSafeZone) { bInSafeZone = bNewInSafeZone; }
	
#pragma endregion

#pragma region HealthInterface Implementations
public:
	virtual bool IsDead_Implementation() const override;
	virtual float GetHealthPercentage_Implementation() const override;
	virtual bool CanRegenerate_Implementation() override;
#pragma endregion

#pragma region CharacterDefenceInterface Implementations
public:
	virtual bool IsInSafeZone_Implementation() const override;
	virtual void SetIsInSafeZone_Implementation(const bool bNewInSafeZone) override;
	virtual float GetArmourAmount_Implementation() const override;
	
#pragma endregion
};

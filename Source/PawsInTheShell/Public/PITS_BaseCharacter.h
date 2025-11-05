// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PITS_BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
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

	UPROPERTY(EditAnywhere, meta=(ToolTip="Rate at which the character regenerates health over time (health per second)"))
	float HealthRegenerationRate = 0.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Maximum speed of the character when moving"))
	float MaxWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Initial velocity (instantaneous force acceleration) applied when the character jumps"))
	float JumpZVelocity = 500.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Amount of lateral control for the character while in air (0 = no control, 1 = full control (Max Walk Speed)"))
	float AirControl = 0.35f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Deceleration applied when walking and not applying acceleration"))
	float BrakingDecelerationWalking = 0.35f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="LAteral deceleration applied when falling and not applying acceleration"))
	float BrakingDecelerationFalling = 0.35f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Additional notes about this character (for development purposes)"))
	FString Notes;

};

UCLASS(Abstract)
class PAWSINTHESHELL_API APITS_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Camera boom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

public:
	APITS_BaseCharacter();

protected:

	/** Data on the type of picked weapon and visuals of this pickup */
	UPROPERTY(EditAnywhere, Category="Character|Stats")
	FDataTableRowHandle CharacterStatsType;

	UPROPERTY()
	float CurrentHealth = 1.0f;

	UPROPERTY()
	float MaxHealth = 1.0f;

	UPROPERTY()
	float HealthRegenerationRate = 0.0f;

	UPROPERTY()
	FText CharacterName = FText::FromString("Unnamed");

	/** Timer to regenerate health */
	FTimerHandle RegenerationTimer;

	void RegenerateHealth();

	/** Native construction script */
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Gameplay Initialization*/
	virtual void BeginPlay() override;

	/** Gameplay cleanup */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
	
	/** Called from Input Actions for movement input */
	void Move(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void Look(const FInputActionValue& Value);
	
	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoShoot();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoChangeWeapon();

#pragma region Property Getters
public:
	
	/** Returns the camera component **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns the camera boom component **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/* Returns the character name from the data table */
	FORCEINLINE FText GetCharacterName() const { return CharacterName; }

	/* Returns the current health of the character */
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/* Returns the maximum health of the character */
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
#pragma endregion 
};

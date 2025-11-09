// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BaseCharacter.h"
#include "Interfaces/PITS_SafeZoneInterface.h"
#include "PITS_BasePlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

UCLASS(Abstract, BlueprintType, Blueprintable)
class PAWSINTHESHELL_API APITS_BasePlayerCharacter : public APITS_BaseCharacter, public IPITS_SafeZoneInterface
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Camera boom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

public:
	APITS_BasePlayerCharacter();

protected:
	
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

#pragma endregion

#pragma region SafeZoneInterface Implementations
public:
	virtual bool IsInSafeZone_Implementation() const override;
	virtual void SetIsInSafeZone_Implementation(const bool bNewInSafeZone) override;
#pragma endregion
};

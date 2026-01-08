// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PawsInTheShell/Public/PITS_BasePlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Utils/PITS_Logs.h"
#include "Subsystems/PITS_WorldSubsystem.h"
#include "Structs/PITS_WeaponDataTableRow.h"
#include "Utils/PITS_Globals.h"

// Sets default values
APITS_BasePlayerCharacter::APITS_BasePlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(50.f, 140.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	CameraBoom->TargetArmLength = 1000.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 250.0f);
	CameraBoom->bUsePawnControlRotation = true;
	
	// Create the Camera Component	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetCameraBoom());
	FollowCamera->bUsePawnControlRotation = false;

	// Set the mesh location and rotation
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -135.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 2700.f, 0.0f));

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Assign the player tag
	Tags.Add(UPITS_Globals::GetPlayerTag());
}

void APITS_BasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Sets the ammo amounts based on weapon data table
	if (const FPITS_WeaponDataTableRow* WeaponData = WeaponStatsType.GetRow<FPITS_WeaponDataTableRow>(FString()))
	{
		AmmoAmount = MaxAmmoAmount = WeaponData->MaxAmmo;
	}
}

void APITS_BasePlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APITS_BasePlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APITS_BasePlayerCharacter::JumpStart()
{
	Jump();
}

void APITS_BasePlayerCharacter::JumpEnd()
{
	StopJumping();
}

void APITS_BasePlayerCharacter::Shoot()
{
	if (bCoolingDown)
	{
		UE_LOG(LogPITS, Warning, TEXT("'%s' Cannot shoot while cooling down!"), *GetNameSafe(this));
		return;
	}
	if (AmmoAmount <= 0)
	{
		UE_LOG(LogPITS, Warning, TEXT("'%s' Cannot shoot with no ammo!"), *GetNameSafe(this));
		return;
	}
	AmmoAmount--;
	if (const UPITS_WorldSubsystem* WorldSubsystem = Controller->GetWorld()->GetSubsystem<UPITS_WorldSubsystem>())
	{
		WorldSubsystem->NotifyUpdateWeapon();
	}

	UE_LOG(LogPITS, Log, TEXT("'%s' Shooting! Ammo left: %d / %d"), *GetNameSafe(this), AmmoAmount, MaxAmmoAmount);

	// Will play montage only if character is still
	if (ShootMontage && GetMesh() && GetVelocity().Size() <= KINDA_SMALL_NUMBER)
	{
		PlayAnimMontage(ShootMontage);
	}
	if (GetWeaponCooldownTime() > 0.f)
	{
		bCoolingDown = true;
		FTimerHandle CooldownTimerHandle;
		GetWorldTimerManager().SetTimer(CooldownTimerHandle, [this]()
		{
			bCoolingDown = false;
			UE_LOG(LogPITS, Log, TEXT("'%s' Weapon cooldown finished."), *GetNameSafe(this));
		}, GetWeaponCooldownTime(), false);
	}
	HandleShoot();
}

void APITS_BasePlayerCharacter::ChangeCharacter()
{
	// Check if actor is moving or falling
	if (GetVelocity().Size() > KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogPITS, Warning, TEXT("'%s' Cannot change character while moving or jumping!"), *GetNameSafe(this));
		return;
	}
	if (!bInSafeZone)
	{
		UE_LOG(LogPITS, Warning, TEXT("'%s' Cannot change character while outside safe zone!"), *GetNameSafe(this));
		return;
	}
	if (const UPITS_WorldSubsystem* WorldSubsystem = Controller->GetWorld()->GetSubsystem<UPITS_WorldSubsystem>())
	{
		WorldSubsystem->ChangeCharacter();
	}
}

void APITS_BasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APITS_BasePlayerCharacter::JumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APITS_BasePlayerCharacter::JumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APITS_BasePlayerCharacter::Move);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APITS_BasePlayerCharacter::Look);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APITS_BasePlayerCharacter::Look);

		// Additional bindings
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &APITS_BasePlayerCharacter::Shoot);
		EnhancedInputComponent->BindAction(ChangeCharacterAction, ETriggerEvent::Triggered, this, &APITS_BasePlayerCharacter::ChangeCharacter);
	}
	else
	{
		UE_LOG(LogPITS, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
	}
}

#pragma region SafeZoneEligibleInterface Implementations
bool APITS_BasePlayerCharacter::IsInSafeZone_Implementation() const
{
	return bInSafeZone;
}

void APITS_BasePlayerCharacter::SetIsInSafeZone_Implementation(const bool bNewInSafeZone)
{
	bInSafeZone = bNewInSafeZone;
}
#pragma endregion

#pragma region AmmoInterface Implementations
int APITS_BasePlayerCharacter::GetCurrentAmmoAmount_Implementation() const
{
	return AmmoAmount;
}

int APITS_BasePlayerCharacter::GetMaxAmmoAmount_Implementation() const
{
	return MaxAmmoAmount;
}

int APITS_BasePlayerCharacter::AddAmmo_Implementation(const int Amount)
{
	UE_LOG(LogPITS, Log, TEXT("'%s' Adding %d ammo. Current: %d / Max: %d"), *GetNameSafe(this), Amount, AmmoAmount, MaxAmmoAmount);
	if (MaxAmmoAmount < 0)
	{
		return AmmoAmount;
	}
	AmmoAmount = FMath::Clamp(AmmoAmount + Amount, 0, MaxAmmoAmount);
	UE_LOG(LogPITS, Log, TEXT("'%s' New ammo amount: %d / %d"), *GetNameSafe(this), AmmoAmount, MaxAmmoAmount);
	return AmmoAmount;
}
#pragma endregion

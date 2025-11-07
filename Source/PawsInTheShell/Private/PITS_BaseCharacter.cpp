// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PawsInTheShell/Public/PITS_BaseCharacter.h"

#include "DamageType_Regeneration.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Utils/PITS_Logs.h"
#include "PITS_WorldSubsystem.h"
#include "Components/PITS_HealthComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
APITS_BaseCharacter::APITS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Health = CreateDefaultSubobject<UPITS_HealthComponent>(FName("Health"));
	
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
}

float APITS_BaseCharacter::TakeDamage(const float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageEvent.DamageTypeClass != nullptr)
	{
		// Check if damage type is regeneration
		if (DamageEvent.DamageTypeClass->IsChildOf(UDamageType_Regeneration::StaticClass()))
		{
			Health->AddHealth(DamageAmount);
		}
	}
	else
	{
		const float EffectiveDamage = FMath::Max(0.0f, DamageAmount - ArmourAmount);
		Health->RemoveHealth(EffectiveDamage);
		return EffectiveDamage;
	}
	return 0.0f;
}

void APITS_BaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (const FCharacterDataTableRow* Data = CharacterStatsType.GetRow<FCharacterDataTableRow>(FString()))
	{
		GetCharacterMovement()->JumpZVelocity = Data->JumpZVelocity;
		GetCharacterMovement()->AirControl = Data->AirControl;
		GetCharacterMovement()->MaxWalkSpeed = Data->MaxWalkSpeed;
		GetCharacterMovement()->BrakingDecelerationWalking = Data->BrakingDecelerationWalking;
		GetCharacterMovement()->BrakingDecelerationFalling = Data->BrakingDecelerationFalling;

		Health->SetMaxHealth(Data->MaxHealth);
		Health->SetCurrentHealth(Data->StartingHealth);
		Health->SetCanRegenerate(Data->bCanRegenerate);
		
		ArmourAmount = Data->ArmourAmount;
		CharacterName = Data->CharacterName;
	}
}

void APITS_BaseCharacter::Move(const FInputActionValue& Value)
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

void APITS_BaseCharacter::Look(const FInputActionValue& Value)
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

void APITS_BaseCharacter::JumpStart()
{
	Jump();
}

void APITS_BaseCharacter::JumpEnd()
{
	StopJumping();
}

void APITS_BaseCharacter::Shoot()
{
}

void APITS_BaseCharacter::ChangeCharacter()
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
	if (UPITS_WorldSubsystem* WorldSubsystem = Controller->GetWorld()->GetSubsystem<UPITS_WorldSubsystem>())
	{
		WorldSubsystem->ChangeCharacter();
	}
}

void APITS_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APITS_BaseCharacter::JumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APITS_BaseCharacter::JumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APITS_BaseCharacter::Move);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APITS_BaseCharacter::Look);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APITS_BaseCharacter::Look);

		// Additional bindings
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &APITS_BaseCharacter::Shoot);
		EnhancedInputComponent->BindAction(ChangeCharacterAction, ETriggerEvent::Triggered, this, &APITS_BaseCharacter::ChangeCharacter);
	}
	else
	{
		UE_LOG(LogPITS, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
	}
}

#pragma region HealthInterface Implementations
bool APITS_BaseCharacter::IsDead_Implementation() const
{
	return Health->IsDead();
}

float APITS_BaseCharacter::GetHealthPercentage_Implementation() const
{
	return Health->GetHealthPercentage();
}

bool APITS_BaseCharacter::CanRegenerate_Implementation()
{
	return Health->CanRegenerate() && !IsDead_Implementation() && Health->GetHealthPercentage() > 0.0f;
}
#pragma endregion

#pragma region CharacterDefenceInterface Implementations
bool APITS_BaseCharacter::IsInSafeZone_Implementation() const
{
	return bInSafeZone;
}

float APITS_BaseCharacter::GetArmourAmount_Implementation() const
{
	return ArmourAmount;
}

void APITS_BaseCharacter::SetIsInSafeZone_Implementation(const bool bNewInSafeZone)
{
	bInSafeZone = bNewInSafeZone;
}
#pragma endregion

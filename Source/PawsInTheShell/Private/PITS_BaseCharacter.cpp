// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PawsInTheShell/Public/PITS_BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PawsInTheShell/Public/Utils/PITS_Logs.h"
#include "PITS_WorldSubsystem.h"

// Sets default values
APITS_BaseCharacter::APITS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

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

		MaxHealth = Data->MaxHealth;
		CurrentHealth = FMath::Clamp(CurrentHealth, 1.0f, MaxHealth);
		HealthRegenerationRate = FMath::Clamp(HealthRegenerationRate, 0.0f, MaxHealth);

		CharacterName = Data->CharacterName;
	}
}

void APITS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthRegenerationRate > 0.0f)
	{
		// start the health regeneration timer
		GetWorld()->GetTimerManager().SetTimer(RegenerationTimer, this, &APITS_BaseCharacter::RegenerateHealth, 1.0f, true);
	}
}

void APITS_BaseCharacter::RegenerateHealth()
{
	UE_LOG(LogPITS, Log, TEXT("'%s' Regenerating Health: %f + %f"), *GetNameSafe(this), CurrentHealth, HealthRegenerationRate);
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthRegenerationRate, 0.0f, MaxHealth);
}


void APITS_BaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the respawn timer
	GetWorld()->GetTimerManager().ClearTimer(RegenerationTimer);
}

void APITS_BaseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void APITS_BaseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void APITS_BaseCharacter::DoMove(const float Right, const float Forward)
{
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
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void APITS_BaseCharacter::DoLook(const float Yaw, const float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void APITS_BaseCharacter::DoJumpStart()
{
	Jump();
}

void APITS_BaseCharacter::DoJumpEnd()
{
	StopJumping();
}

void APITS_BaseCharacter::DoShoot()
{
}

void APITS_BaseCharacter::DoChangeCharacter()
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APITS_BaseCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APITS_BaseCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APITS_BaseCharacter::Move);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APITS_BaseCharacter::Look);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APITS_BaseCharacter::Look);

		// Additional bindings
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &APITS_BaseCharacter::DoShoot);
		EnhancedInputComponent->BindAction(ChangeCharacterAction, ETriggerEvent::Triggered, this, &APITS_BaseCharacter::DoChangeCharacter);
	}
	else
	{
		UE_LOG(LogPITS, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
	}
}

#pragma region HealthInterface Implementations
bool APITS_BaseCharacter::IsDead_Implementation() const
{
	return CurrentHealth <= 0.0f;
}

float APITS_BaseCharacter::GetCurrentHealth_Implementation() const
{
	return CurrentHealth;
}

float APITS_BaseCharacter::GetMaxHealth_Implementation() const
{
	return MaxHealth;
}

float APITS_BaseCharacter::GetHealthPercentage_Implementation() const
{
	return (MaxHealth > 0.0f) ? (CurrentHealth / MaxHealth) : 0.0f;
}
#pragma endregion
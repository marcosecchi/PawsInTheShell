// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_BaseProjectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Subsystems/PITS_WorldSubsystem.h"
#include "Utils/PITS_Globals.h"
#include "Utils/PITS_Logs.h"

// Sets default values
APITS_BaseProjectile::APITS_BaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 0.0f;
	
	// create the collision component and assign it as the root
	RootComponent = CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));

	CollisionComponent->SetCapsuleSize(32.0f, 8.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	// create the mesh component and attach it to the root component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// create the projectile movement component. No need to attach it because it's not a Scene Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	
	// set the default damage type
	HitDamageType = UDamageType::StaticClass();
	
	// Assign the bullet tag
	Tags.Add(UPITS_Globals::GetBulletTag());
}

// Called when the game starts or when spawned
void APITS_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	// ignore the pawn that shot this projectile
	CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void APITS_BaseProjectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogPITS, Log, TEXT("'%s' hit '%s'."), *GetNameSafe(this), *GetNameSafe(Other));
	
	ProcessHit(Other, OtherComp, Hit.ImpactPoint, -Hit.ImpactNormal);

	HandleProjectileHit(Hit);

	OnDestruction();
}

void APITS_BaseProjectile::ProcessHit(AActor* HitActor, UPrimitiveComponent* HitComp, const FVector& HitLocation,
	const FVector& HitDirection)
{
	UGameplayStatics::ApplyDamage(HitActor, HitDamage, GetInstigator()->GetController(), this, HitDamageType);

	// have we hit a physics object?
	if (HitComp->IsSimulatingPhysics())
	{
		// give some physics impulse to the object
		HitComp->AddImpulseAtLocation(HitDirection * PhysicsForce, HitLocation);
	}
}

void APITS_BaseProjectile::OnDestruction()
{
	UPITS_WorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UPITS_WorldSubsystem>();
	CHECK_PTR_AND_LOG(WorldSubsystem);
	if (WorldSubsystem->IsObjectPooled(this))
	{
		WorldSubsystem->ReleasePooledObject(this);
	}
	else
	{
		Destroy();
	}
}

void APITS_BaseProjectile::HandleAfterAcquire_Implementation()
{
	UE_LOG(LogPITS, Log, TEXT("'%s' Acquired from pool."), *GetNameSafe(this));
	
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Deactivate();
		ProjectileMovement->ResetInterpolation();
	
		SetActorRotation(GetActorForwardVector().Rotation());
	
		ProjectileMovement->SetUpdatedComponent(CollisionComponent);
		ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->MaxSpeed;

		ProjectileMovement->Activate(true);
		ProjectileMovement->ResetInterpolation();
		ProjectileMovement->SetComponentTickEnabled(true);
	}
}

void APITS_BaseProjectile::HandleRelease_Implementation()
{
	ProjectileMovement->StopMovementImmediately();
	UE_LOG(LogPITS, Log, TEXT("'%s' Released to pool."), *GetNameSafe(this));
}


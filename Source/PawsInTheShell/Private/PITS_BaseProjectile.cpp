// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_BaseProjectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Subsystems/PITS_ObjectPoolSubsystem.h"
#include "Utils/PITS_Globals.h"
#include "Utils/PITS_Logs.h"
#include "Structs/PITS_ProjectileDataTableRow.h"

void APITS_BaseProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (const FPITS_ProjectileDataTableRow* ProjectileData = ProjectileDataType.GetRow<FPITS_ProjectileDataTableRow>(FString()))
	{
		if (ProjectileData->ProjectileMesh)
		{
			Mesh->SetStaticMesh(ProjectileData->ProjectileMesh);
		}
		HitDamage = ProjectileData->Damage;
		PhysicsForce = ProjectileData->PhysicsForce;
		HitDamageType = ProjectileData->DamageType;
		ProjectileName = ProjectileData->ProjectileName;
		ProjectileMovement->InitialSpeed = ProjectileData->Speed;
		ProjectileMovement->MaxSpeed = ProjectileData->Speed;
	}
}

APITS_BaseProjectile::APITS_BaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 0.0f;
	
	RootComponent = CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	CollisionComponent->InitBoxExtent(FVector(150.0f, 50.0f, 50.0f));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	
	HitDamageType = UDamageType::StaticClass();
	Tags.Add(UPITS_Globals::GetBulletTag());
}

void APITS_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	// Ignore collision with instigator
	if (!bDamageOwner && GetInstigator())
	{
		CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	}
}

void APITS_BaseProjectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogPITS, Log, TEXT("'%s' hit '%s'."), *GetNameSafe(this), *GetNameSafe(Other));
	
	ProcessHit(Other, OtherComp, Hit.ImpactPoint, -Hit.ImpactNormal);

	HandleProjectileHit(Hit);

	OnHitComplete();
}

void APITS_BaseProjectile::ProcessHit(AActor* HitActor, UPrimitiveComponent* HitComp, const FVector& HitLocation,
	const FVector& HitDirection)
{
	UGameplayStatics::ApplyDamage(HitActor, HitDamage, nullptr, this, HitDamageType);

	// Apply physics impulse if applicable
	if (HitComp->IsSimulatingPhysics())
	{
		HitComp->AddImpulseAtLocation(HitDirection * PhysicsForce, HitLocation);
	}
}

void APITS_BaseProjectile::OnHitComplete()
{
	// Check if this projectile is pooled
	UPITS_ObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UPITS_ObjectPoolSubsystem>();
	CHECK_PTR_AND_LOG(PoolSubsystem);
	// Release to pool or destroy
	if (PoolSubsystem->IsObjectPooled(this))
	{
		PoolSubsystem->ReleasePooledObject(this);
	}
	else
	{
		Destroy();
	}
}

void APITS_BaseProjectile::HandleAcquire_Implementation()
{
	UE_LOG(LogPITS, Log, TEXT("'%s' Acquired from pool."), *GetNameSafe(this));
	
	if (ProjectileMovement && CollisionComponent)
	{
		// stop and prepare movement component
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Deactivate();

		// ensure the movement component updates the correct component
		ProjectileMovement->SetUpdatedComponent(CollisionComponent);

		// compute a safe direction and set starting velocity using InitialSpeed
		const FVector Direction = GetActorForwardVector().GetSafeNormal();
		ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;

		// activate and reset interpolation once, enable ticking
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


// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_SafeZone.h"

#include "NiagaraFunctionLibrary.h"
#include "PITS_BasePlayerCharacter.h"
#include "PITS_BaseProjectile.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PITS_SafeZoneEligibleInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/PITS_Logs.h"

void APITS_SafeZone::HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Check if overlapping actor implements the Defence Interface
	if (OtherActor->GetClass()->ImplementsInterface(UPITS_SafeZoneEligibleInterface::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Actor '%s' entered a safe zone"), *GetNameSafe(this), *GetNameSafe(OtherActor));
		IPITS_SafeZoneEligibleInterface::Execute_SetIsInSafeZone(OtherActor, true);
	}
	// Check if overlapping actor is a APITS_BaseProjectile class
	if (OtherActor->IsA(APITS_BaseProjectile::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Projectile '%s' entered a safe zone and will be destroyed"), *GetNameSafe(this), *GetNameSafe(OtherActor));
		const FVector SpawnLocation = OtherActor->GetActorLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			VfxHit,
			SpawnLocation,
			FRotator::ZeroRotator,
			FVector::OneVector,
			true,
			true
		);


		OtherActor->Destroy();
	}
}

void APITS_SafeZone::HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Check if actor implements the Defence Interface
	if (OtherActor->GetClass()->ImplementsInterface(UPITS_SafeZoneEligibleInterface::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Actor '%s' exited a safe zone"), *GetNameSafe(this), *GetNameSafe(OtherActor));
		IPITS_SafeZoneEligibleInterface::Execute_SetIsInSafeZone(OtherActor, false);
	}
}

APITS_SafeZone::APITS_SafeZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetArrowColor(FLinearColor(0.0f, 1.0f, 1.0f));
	Arrow->SetArrowLength(160.0f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SafeZoneVolume = CreateDefaultSubobject<USphereComponent>(TEXT("SafeZoneVolume"));
	SafeZoneVolume->SetupAttachment(RootComponent);
	SafeZoneVolume->SetSphereRadius(500.0f);
	OnActorBeginOverlap.AddDynamic(this, &APITS_SafeZone::HandleActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &APITS_SafeZone::HandleActorEndOverlap);
}

void APITS_SafeZone::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogPITS, Log, TEXT("'%s' Performing initial safe zone overlap check for all player characters..."), *GetNameSafe(this));
	TArray<AActor*> PlayerCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APITS_BasePlayerCharacter::StaticClass(), PlayerCharacters);
	for (AActor* PlayerCharacter : PlayerCharacters)
	{
		if (
			PlayerCharacter->GetClass()->ImplementsInterface(UPITS_SafeZoneEligibleInterface::StaticClass()) &&
			FVector::Distance(PlayerCharacter->GetActorLocation(), GetActorLocation()) < SafeZoneVolume->GetScaledSphereRadius())
		{
			IPITS_SafeZoneEligibleInterface::Execute_SetIsInSafeZone(PlayerCharacter, true);
			UE_LOG(LogPITS, Log, TEXT("'%s' Initial overlap check for actor '%s': Inside safe zone"), *GetNameSafe(this), *GetNameSafe(PlayerCharacter));
		}
	}
}
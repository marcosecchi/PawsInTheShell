// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_BaseTrigger.h"

#include "PITS_BasePlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Utils/PITS_Globals.h"
#include "Utils/PITS_Logs.h"

// Sets default values
APITS_BaseTrigger::APITS_BaseTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	// create the root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// create the collision sphere
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(RootComponent);

	SphereCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 84.0f));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECC_WorldStatic);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->bFillCollisionUnderneathForNavmesh = true;

	// subscribe to the collision overlap on the sphere
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APITS_BaseTrigger::HandleActorBeginOverlap);

	// create the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionProfileName(FName("NoCollision"));

	// Assign the pickup tag
	Tags.Add(UPITS_Globals::GetPickupTag());
}

void APITS_BaseTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the respawn timer
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void APITS_BaseTrigger::HandleActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag(UPITS_Globals::GetPlayerTag()))
	{
		UE_LOG(LogPITS, Warning, TEXT("'%s' Overlapped by non-player tagged actor '%s', ignoring"), *GetNameSafe(this), *GetNameSafe(OtherActor));
		return;
	}
		
	if (APITS_BasePlayerCharacter* OverlappedCharacter = Cast<APITS_BasePlayerCharacter>(OtherActor))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Character '%s' overlapped a pickup"), *GetNameSafe(this), *GetNameSafe(OverlappedCharacter));
		// Deactivate the pickup
		SetTriggerActive(false);

		if (bWillRespawn)
		{
			// Starts the respawn timer
			GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &APITS_BaseTrigger::Respawn, RespawnTime, false);
		}

		// call the BP handler
		HandleTrigger(OverlappedCharacter);
	}
}

void APITS_BaseTrigger::Respawn()
{
	UE_LOG(LogPITS, Log, TEXT("'%s' Respawning pickup"), *GetNameSafe(this));
	// Reactivate the pickup
	SetTriggerActive(true);

	// call the BP handler
	HandleRespawn();
}

void APITS_BaseTrigger::SetTriggerActive(const bool bIsActive)
{
	// unhide this pickup
	SetActorHiddenInGame(!bIsActive);

	// enable collision
	SetActorEnableCollision(bIsActive);
}

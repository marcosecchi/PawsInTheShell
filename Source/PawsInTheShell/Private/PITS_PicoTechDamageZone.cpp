// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_PicoTechDamageZone.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Damage/PITS_DamageType_PicoTech.h"
#include "Engine/DamageEvents.h"
#include "Utils/PITS_Logs.h"

APITS_PicoTechDamageZone::APITS_PicoTechDamageZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetArrowColor(FLinearColor(1.0f, 0.0f, 1.0f));
	Arrow->SetArrowLength(160.0f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	DamageZoneVolume = CreateDefaultSubobject<USphereComponent>(TEXT("DamageZoneVolume"));
	DamageZoneVolume->SetupAttachment(RootComponent);
	DamageZoneVolume->SetSphereRadius(500.0f);
	OnActorBeginOverlap.AddDynamic(this, &APITS_PicoTechDamageZone::HandleActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &APITS_PicoTechDamageZone::HandleActorEndOverlap);
}

void APITS_PicoTechDamageZone::HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (DamagedActors.Contains(OtherActor)) return;
	DamagedActors.Add(OtherActor);
	UE_LOG(LogPITS, Log, TEXT("'%s' Started PicoTech damage for Actor '%s'"), *GetNameSafe(this), *GetNameSafe(OtherActor));
	if (!GetWorld()->GetTimerManager().IsTimerActive(DamageTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(DamageTimer, this, &APITS_PicoTechDamageZone::ProvokeDamage, 1.0f, true);
	}
}

void APITS_PicoTechDamageZone::HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (DamagedActors.Contains(OtherActor))
	{
		DamagedActors.Remove(OtherActor);
		UE_LOG(LogPITS, Log, TEXT("'%s' Stopped PicoTech damage for Actor '%s'"), *GetNameSafe(this), *GetNameSafe(OtherActor));
	}
	if (DamagedActors.Num() == 0)
	{
		StopDamaging();
	}
}

void APITS_PicoTechDamageZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	StopDamaging();
}

void APITS_PicoTechDamageZone::ProvokeDamage()
{
	UE_LOG(LogPITS, Log, TEXT("'%s' Damaging %d Actors"), *GetNameSafe(this), DamagedActors.Num());
	for (AActor* RegeneratingActor : DamagedActors)
	{
		FDamageEvent DamageEvent = FDamageEvent();
		DamageEvent.DamageTypeClass = UPITS_DamageType_PicoTech::StaticClass();
		RegeneratingActor->TakeDamage(DamageAmount, DamageEvent, nullptr, this);
		UE_LOG(LogPITS, Log, TEXT("'%s' Damaging Actor '%s'"), *GetNameSafe(this), *GetNameSafe(RegeneratingActor));
	}
}

void APITS_PicoTechDamageZone::StopDamaging()
{
	GetWorld()->GetTimerManager().ClearTimer(DamageTimer);
}

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_RegenerationZone.h"

#include "PITS_DamageType_Regeneration.h"
#include "Engine/DamageEvents.h"
#include "Interfaces/PITS_HealthInterface.h"
#include "Utils/PITS_Logs.h"

void APITS_RegenerationZone::HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::HandleActorBeginOverlap(OverlappedActor, OtherActor);

	// Check if actor implements the Health Interface
	if (OtherActor->GetClass()->ImplementsInterface(UPITS_HealthInterface::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Actor '%s' entered a regeneration zone."), *GetNameSafe(this), *GetNameSafe(OtherActor));
		if (IPITS_HealthInterface::Execute_CanRegenerate(OtherActor))
		{
			if (RegeneratingActors.Contains(OtherActor)) return;
			RegeneratingActors.Add(OtherActor);
			UE_LOG(LogPITS, Log, TEXT("'%s' Started Health Regeneration for Actor '%s'"), *GetNameSafe(this), *GetNameSafe(OtherActor));
			if (!GetWorld()->GetTimerManager().IsTimerActive(RegenerationTimer))
			{
				GetWorld()->GetTimerManager().SetTimer(RegenerationTimer, this, &APITS_RegenerationZone::RegenerateHealth, 1.0f, true);
			}
		}
	}
}

void APITS_RegenerationZone::HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::HandleActorEndOverlap(OverlappedActor, OtherActor);
	
	// Check if actor implements the Health Interface
	if (OtherActor->GetClass()->ImplementsInterface(UPITS_HealthInterface::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Actor '%s' exited  a regeneration zone."), *GetNameSafe(this), *GetNameSafe(OtherActor));
		if (IPITS_HealthInterface::Execute_CanRegenerate(OtherActor))
		{
			if (RegeneratingActors.Contains(OtherActor))
			{
				RegeneratingActors.Remove(OtherActor);
			UE_LOG(LogPITS, Log, TEXT("'%s' Stopped Health Regeneration for Actor '%s'"), *GetNameSafe(this), *GetNameSafe(OtherActor));
			}
			if (RegeneratingActors.Num() == 0)
			{
				StopRegenerating();
			}
		}
	}
}

void APITS_RegenerationZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	StopRegenerating();
}

void APITS_RegenerationZone::RegenerateHealth()
{
	UE_LOG(LogPITS, Log, TEXT("'%s' Regenerating Health for %d Actors"), *GetNameSafe(this), RegeneratingActors.Num());
	for (AActor* RegeneratingActor : RegeneratingActors)
	{
		FDamageEvent DamageEvent = FDamageEvent();
		DamageEvent.DamageTypeClass = UPITS_DamageType_Regeneration::StaticClass();
		RegeneratingActor->TakeDamage(RegenerationAmount, DamageEvent, nullptr, this);
		UE_LOG(LogPITS, Log, TEXT("'%s' Regenerating Health for Actor '%s'"), *GetNameSafe(this), *GetNameSafe(RegeneratingActor));
	}
}

void APITS_RegenerationZone::StopRegenerating()
{
	// clear the regeneration timer
	GetWorld()->GetTimerManager().ClearTimer(RegenerationTimer);
}

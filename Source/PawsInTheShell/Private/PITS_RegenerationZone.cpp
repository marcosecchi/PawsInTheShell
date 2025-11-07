// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_RegenerationZone.h"

#include "Interfaces/HealthInterface.h"
#include "Utils/PITS_Logs.h"

void APITS_RegenerationZone::HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::HandleActorBeginOverlap(OverlappedActor, OtherActor);

	// Check if actor implements the Health Interface
	if (OtherActor->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Actor '%s' entered a regeneration zone."), *GetNameSafe(this), *GetNameSafe(OtherActor));
		if (IHealthInterface::Execute_CanRegenerate(OtherActor))
		{
			IHealthInterface::Execute_StartRegenerating(OtherActor);
			UE_LOG(LogPITS, Log, TEXT("'%s' Started Health Regeneration for Actor '%s'"), *GetNameSafe(this), *GetNameSafe(OtherActor));
		}
	}
}

void APITS_RegenerationZone::HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::HandleActorEndOverlap(OverlappedActor, OtherActor);
	
	// Check if actor implements the Health Interface
	if (OtherActor->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Actor '%s' exited  a regeneration zone."), *GetNameSafe(this), *GetNameSafe(OtherActor));
		if (IHealthInterface::Execute_CanRegenerate(OtherActor))
		{
			IHealthInterface::Execute_StopRegenerating(OtherActor);
			UE_LOG(LogPITS, Log, TEXT("'%s' Stopped Health Regeneration for Actor '%s'"), *GetNameSafe(this), *GetNameSafe(OtherActor));
		}
	}
}

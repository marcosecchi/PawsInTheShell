// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_ExitZone.h"
#include "Components/SphereComponent.h"
#include "Subsystems/PITS_MissionSubsystem.h"
#include "Utils/PITS_Logs.h"


// Sets default values
APITS_ExitZone::APITS_ExitZone()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bWillRespawn = false;
}

// Called when the game starts or when spawned
void APITS_ExitZone::BeginPlay()
{
	Super::BeginPlay();

	if (!bActiveAtStart)
	{
		GetMesh()->SetVisibility(false);
		GetSphereCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else if (bWaitForMissionsCompletion)
	{
		if (UPITS_MissionSubsystem* MissionSubsystem = GetWorld()->GetSubsystem<UPITS_MissionSubsystem>())
		{
			MissionSubsystem->OnAllMissionsComplete.AddDynamic(this, &APITS_ExitZone::ActivateTrigger);
		}
	}
}

void APITS_ExitZone::HandleTrigger_Implementation(APITS_BasePlayerCharacter* OverlappedCharacter)
{
	UE_LOG(LogPITS, Log, TEXT("Player '%s' reached the Exit Zone"), *GetNameSafe(OverlappedCharacter));
}

void APITS_ExitZone::ActivateTrigger()
{
	UE_LOG(LogPITS, Log, TEXT("Mission complete: Activating Exit Trigger"));
	GetMesh()->SetVisibility(true);
	GetSphereCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


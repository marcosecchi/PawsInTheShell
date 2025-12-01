// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITX_ExitZone.h"
#include "Components/SphereComponent.h"
#include "Subsystems/PITS_MissionSubsystem.h"
#include "Utils/PITS_Logs.h"


// Sets default values
APITX_ExitZone::APITX_ExitZone()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetSphereCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	bWillRespawn = false;
}

// Called when the game starts or when spawned
void APITX_ExitZone::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetVisibility(false);

	if (UPITS_MissionSubsystem* MissionSubsystem = GetWorld()->GetSubsystem<UPITS_MissionSubsystem>())
	{
		MissionSubsystem->OnAllMissionsComplete.AddDynamic(this, &APITX_ExitZone::ActivateTrigger);
	}
}

void APITX_ExitZone::HandleTrigger_Implementation(APITS_BasePlayerCharacter* OverlappedCharacter)
{
	UE_LOG(LogPITS, Log, TEXT("Player '%s' reached the Exit Zone"), *GetNameSafe(OverlappedCharacter));
}

void APITX_ExitZone::ActivateTrigger()
{
	UE_LOG(LogPITS, Log, TEXT("Mission complete: Activating Exit Trigger"));
	GetMesh()->SetVisibility(true);
	GetSphereCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_MissionPickup.h"
#include "Components/PITS_MissionHolder.h"
#include "Subsystems/PITS_MissionSubsystem.h"

APITS_MissionPickup::APITS_MissionPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MissionHolderComponent = CreateDefaultSubobject<UPITS_MissionHolder>(TEXT("MissionHolder"));
}

void APITS_MissionPickup::HandleTrigger_Implementation(APITS_BasePlayerCharacter* OverlappedCharacter)
{
	if(UPITS_MissionSubsystem* MissionSubsystem = GetWorld()->GetSubsystem<UPITS_MissionSubsystem>())
	{
		MissionSubsystem->UpdateMission(MissionHolderComponent->GetMission(), MissionHolderComponent->GetMissionIncrement());
	}
}


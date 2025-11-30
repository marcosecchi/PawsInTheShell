// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Components/PITS_MissionHolder.h"

#include "Subsystems/PITS_MissionSubsystem.h"

UPITS_MissionHolder::UPITS_MissionHolder()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPITS_MissionHolder::DispatchMissionUpdate()
{
	if (UPITS_MissionSubsystem *MissionSubsystem = GetWorld()->GetSubsystem<UPITS_MissionSubsystem>())
	{
		MissionSubsystem->UpdateMission(Mission, MissionIncrement);
	}
}


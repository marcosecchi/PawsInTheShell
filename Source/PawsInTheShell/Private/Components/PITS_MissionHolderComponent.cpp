// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Components/PITS_MissionHolderComponent.h"

#include "Subsystems/PITS_MissionSubsystem.h"

UPITS_MissionHolderComponent::UPITS_MissionHolderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPITS_MissionHolderComponent::DispatchMissionUpdate()
{
	if (UPITS_MissionSubsystem *MissionSubsystem = GetWorld()->GetSubsystem<UPITS_MissionSubsystem>())
	{
		MissionSubsystem->UpdateMission(Mission, MissionIncrement);
	}
}


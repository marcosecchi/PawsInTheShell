// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_GameModeBase.h"

#include "Subsystems/PITS_MissionSubsystem.h"

void APITS_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	MissionSubsystem = GetWorld()->GetSubsystem<UPITS_MissionSubsystem>();
	if(MissionSubsystem && MissionList.Num() > 0)
	{
		MissionSubsystem->InitializeMissions(MissionList);
	}
}

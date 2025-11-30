// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Subsystems/PITS_MissionSubsystem.h"

void UPITS_MissionSubsystem::InitializeMissions(const TArray<UPITS_MissionDataAsset*> Missions)
{
	// Clear any existing missions
	ActiveMissions.Empty();
	// Add new missions
	ActiveMissions.Append(Missions);
	
	// Initialize mission map
	MissionMap.Empty();
	for (UPITS_MissionDataAsset* Mission : ActiveMissions)
	{
		if (Mission)
		{
			MissionMap.Add(Mission, 0);
		}
	}
	
	OnMissionsInitialize.Broadcast();
}

void UPITS_MissionSubsystem::UpdateMission(UPITS_MissionDataAsset* Mission, uint8 ProgressIncrement)
{
	// Check if the mission exists in the map
	if (MissionMap.Contains(Mission))
	{
		// Update the mission progress
		MissionMap[Mission]++;
		
		// Check if the mission is complete
		if (MissionMap[Mission] >= Mission->CompleteCount)
		{
			// Notify mission complete and perform cleanup
			OnMissionComplete.Broadcast(Mission);
			MissionMap.Remove(Mission);
			ActiveMissions.Remove(Mission);
			
			// Check if all missions are complete
			if (ActiveMissions.Num() == 0)
			{
				// Notify all missions complete
				OnAllMissionsComplete.Broadcast();
			}
		}
		else
		{
			// Notify mission update
			OnMissionUpdate.Broadcast(Mission, ProgressIncrement);
		}
	}
}

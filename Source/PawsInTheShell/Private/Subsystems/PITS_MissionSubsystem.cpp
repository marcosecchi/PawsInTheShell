// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Subsystems/PITS_MissionSubsystem.h"

#include "Utils/PITS_Logs.h"

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
	
	UE_LOG(LogPITS, Log, TEXT("'%s' Initialized %d Missions"), *GetNameSafe(this), ActiveMissions.Num());
	OnMissionsInitialize.Broadcast();
}

void UPITS_MissionSubsystem::UpdateMission(UPITS_MissionDataAsset* Mission, const int ProgressIncrement)
{
	// Check if the mission exists in the map
	if (MissionMap.Contains(Mission))
	{
		// Update the mission progress
		MissionMap[Mission] += ProgressIncrement;
		MissionMap[Mission] = FMath::Clamp(MissionMap[Mission], 0, Mission->CompleteCount);
		
		// Check if the mission is complete
		if (MissionMap[Mission] >= Mission->CompleteCount)
		{
			// Notify mission complete and perform cleanup
			UE_LOG(LogPITS, Log, TEXT("'%s' Mission '%s' completed"), *GetNameSafe(this), *GetNameSafe(Mission));
			OnMissionComplete.Broadcast(Mission);
			MissionMap.Remove(Mission);
			ActiveMissions.Remove(Mission);
			
			// Check if all missions are complete
			if (ActiveMissions.Num() == 0)
			{
				UE_LOG(LogPITS, Log, TEXT("'%s' All missions completed"), *GetNameSafe(this));
				// Notify all missions complete
				OnAllMissionsComplete.Broadcast();
			}
		}
		else
		{
			// Notify mission update
			UE_LOG(LogPITS, Log, TEXT("'%s' Mission '%s' updated: %d/%d"), *GetNameSafe(this), *GetNameSafe(Mission), MissionMap[Mission], Mission->CompleteCount);
			OnMissionUpdate.Broadcast(Mission, ProgressIncrement);
		}
	}
}

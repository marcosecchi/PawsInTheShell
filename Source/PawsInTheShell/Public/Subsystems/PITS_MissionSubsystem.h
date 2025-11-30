// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Data/PITS_MissionDataAsset.h"
#include "Subsystems/WorldSubsystem.h"
#include "PITS_MissionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionComplete, UPITS_MissionDataAsset*, Mission);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionUpdate, UPITS_MissionDataAsset*, Mission, uint8, CurrentProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllMissionsComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMissionsInitialize);

/**
 * 
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_MissionSubsystem final : public UWorldSubsystem
{
	GENERATED_BODY()

	/* List of Active Missions */
	UPROPERTY()
	TArray<UPITS_MissionDataAsset*> ActiveMissions;
	
	/* Map of Missions and their current progress */
	UPROPERTY()
	TMap<UPITS_MissionDataAsset*, uint8> MissionMap;
public:

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|MissionSubsystem")
	void InitializeMissions(const TArray<UPITS_MissionDataAsset*> Missions);

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|MissionSubsystem")
	void UpdateMission(UPITS_MissionDataAsset* Mission, uint8 ProgressIncrement = 1);
	
	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|MissionSubsystem")
	FOnMissionComplete OnMissionComplete;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|MissionSubsystem")
	FOnMissionUpdate OnMissionUpdate;
	
	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|MissionSubsystem")
	FOnMissionsInitialize OnMissionsInitialize;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|MissionSubsystem")
	FOnAllMissionsComplete OnAllMissionsComplete;
};

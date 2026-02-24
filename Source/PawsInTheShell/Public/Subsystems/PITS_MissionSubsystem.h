// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Data/PITS_MissionDataAsset.h"
#include "Subsystems/WorldSubsystem.h"
#include "PITS_MissionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionCompleteSignature, UPITS_MissionDataAsset*, Mission);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionUpdateSignature, UPITS_MissionDataAsset*, Mission, int, CurrentProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllMissionsCompleteSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMissionsInitializeSignature);

/**
 * World subsystem that manages the missions of the game, their progress and completion
 * This subsystem is responsible for managing the missions of the game, their progress and completion.
 * It provides functions to initialize the missions, update their progress and notify when a mission is completed or when all missions are completed.
 * It also provides delegates to notify other systems when a mission is updated or completed.
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_MissionSubsystem final : public UWorldSubsystem
{
	GENERATED_BODY()

	/**
	 * List of active missions.
	 */
	UPROPERTY()
	TArray<UPITS_MissionDataAsset*> ActiveMissionList;

protected:
	/* List of all missions */
	UPROPERTY(BlueprintReadOnly, Category="PawsInTheShell|MissionSubsystem")
	TArray<UPITS_MissionDataAsset*> MissionList;
	
	/* Map of Missions and their current progress */
	UPROPERTY(BlueprintReadOnly, Category="PawsInTheShell|MissionSubsystem")
	TMap<UPITS_MissionDataAsset*, int32> MissionMap;

public:
	/**
	 * Initializes the missions of the game.
	 */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|MissionSubsystem")
	void InitializeMissions(const TArray<UPITS_MissionDataAsset*> Missions);

	/**
	 * Updates the progress of a mission.
	 */
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|MissionSubsystem")
	void UpdateMission(UPITS_MissionDataAsset* Mission, const int32 ProgressIncrement = 1);

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|MissionSubsystem")
	FOnMissionCompleteSignature OnMissionComplete;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|MissionSubsystem")
	FOnMissionUpdateSignature OnMissionUpdate;
	
	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|MissionSubsystem")
	FOnMissionsInitializeSignature OnMissionsInitialize;

	UPROPERTY(BlueprintAssignable, Category="PawsInTheShell|MissionSubsystem")
	FOnAllMissionsCompleteSignature OnAllMissionsComplete;
};

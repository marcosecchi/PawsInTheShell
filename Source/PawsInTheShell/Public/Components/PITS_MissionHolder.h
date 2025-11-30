// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PITS_MissionHolder.generated.h"


class UPITS_MissionDataAsset;

/* *
 * Component that holds a mission reference and can dispatch mission updates
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PAWSINTHESHELL_API UPITS_MissionHolder : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPITS_MissionHolder();

protected:
	// Reference to the mission data asset
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|Missions")
	UPITS_MissionDataAsset* Mission;

	// Amount to increment the mission progress by when dispatching an update
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|Missions")
	uint8 MissionIncrement = 1;
	
public:
	// Dispatch a mission update to the Mission Subsystem
	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Missions")
	void DispatchMissionUpdate();
	
	// Getter for the mission data asset
	UPITS_MissionDataAsset* GetMission() const { return Mission; }
	
};

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Data/PITS_MissionDataAsset.h"
#include "GameFramework/GameModeBase.h"
#include "PITS_GameModeBase.generated.h"

class UPITS_MissionSubsystem;
/**
 * 
 */
UCLASS()
class PAWSINTHESHELL_API APITS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	UPROPERTY()
	UPITS_MissionSubsystem* MissionSubsystem;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PawsInTheShell")
	TArray<UPITS_MissionDataAsset*> MissionList;
	
	virtual void BeginPlay() override;
};

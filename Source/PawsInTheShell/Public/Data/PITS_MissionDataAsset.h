// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PITS_MissionDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Category="PawsInTheShell")
class PAWSINTHESHELL_API UPITS_MissionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPITS_MissionDataAsset();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PawsInTheShell")
	FText MissionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PawsInTheShell")
	FText MissionDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PawsInTheShell")
	uint8 CompleteCount;
};

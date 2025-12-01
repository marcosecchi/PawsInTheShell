// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BaseTrigger.h"
#include "PITS_MissionPickup.generated.h"

class UPITS_MissionHolder;

UCLASS(Abstract)
class PAWSINTHESHELL_API APITS_MissionPickup : public APITS_BaseTrigger
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APITS_MissionPickup();

protected:
	
	UPROPERTY()
	TObjectPtr<UPITS_MissionHolder> MissionHolderComponent;
	
	virtual void HandleTrigger_Implementation(APITS_BasePlayerCharacter* OverlappedCharacter) override;
	
public:
	FORCEINLINE UPITS_MissionHolder* GetMissionHolderComponent() const { return MissionHolderComponent; }
	
};

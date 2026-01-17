// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BaseTrigger.h"
#include "PITS_MissionPickup.generated.h"

class UPITS_MissionHolderComponent;

UCLASS(Abstract)
class PAWSINTHESHELL_API APITS_MissionPickup : public APITS_BaseTrigger
{
	GENERATED_BODY()

public:
	APITS_MissionPickup();

protected:
	
	UPROPERTY(EditAnywhere, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPITS_MissionHolderComponent> MissionHolderComponent;
	
	virtual void HandleTrigger_Implementation(APITS_BasePlayerCharacter* OverlappedCharacter) override;
	
public:
	FORCEINLINE UPITS_MissionHolderComponent* GetMissionHolderComponent() const { return MissionHolderComponent; }
	
};

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BaseTrigger.h"
#include "PITS_ExitZone.generated.h"

UCLASS()
class PAWSINTHESHELL_API APITS_ExitZone : public APITS_BaseTrigger
{
	GENERATED_BODY()

public:
	APITS_ExitZone();

	UFUNCTION(BlueprintCallable, Category="PawsInTheShell|Trigger")
	virtual void ActivateTrigger();

protected:
	virtual void BeginPlay() override;

	virtual void HandleTrigger_Implementation(APITS_BasePlayerCharacter* OverlappedCharacter) override;
	
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|Trigger")
	bool bActiveAtStart = true;

	UPROPERTY(EditAnywhere, Category="PawsInTheShell|Trigger",
		meta = (EditCondition = "!bActiveAtStart", EditConditionHides))
	bool bWaitForMissionsCompletion = true;
	
};

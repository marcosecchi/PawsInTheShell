// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_BaseTrigger.h"
#include "PITX_ExitZone.generated.h"

UCLASS()
class PAWSINTHESHELL_API APITX_ExitZone : public APITS_BaseTrigger
{
	GENERATED_BODY()

public:
	APITX_ExitZone();

protected:
	virtual void BeginPlay() override;

	virtual void HandleTrigger_Implementation(APITS_BasePlayerCharacter* OverlappedCharacter) override;
	
	UFUNCTION()
	virtual void ActivateTrigger();
};

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PawsInTheShell/Public/PITS_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "PawsInTheShell/Public/Utils/PITS_Logs.h"

class UEnhancedInputLocalPlayerSubsystem;

void APITS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (const UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
				UE_LOG(LogPITS, Log, TEXT("'%s' Added Input Mapping Context '%s'"), *GetNameSafe(this), *GetNameSafe(CurrentContext));
			}
		}
	}
}

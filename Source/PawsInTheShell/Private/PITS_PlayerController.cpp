// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "EnhancedInputSubsystems.h"
#include "PawsInTheShell/Public/PITS_PlayerController.h"
#include "PawsInTheShell/Public/PITS_PlayerCameraManager.h"

class UEnhancedInputLocalPlayerSubsystem;

APITS_PlayerController::APITS_PlayerController()
{
	PlayerCameraManagerClass = APITS_PlayerCameraManager::StaticClass();
}

void APITS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}

}

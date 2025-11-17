// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PawsInTheShell/Public/PITS_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "PITS_BasePlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/PITS_StatsWidgetInterface.h"
#include "PawsInTheShell/Public/Utils/PITS_Logs.h"

class UEnhancedInputLocalPlayerSubsystem;

void APITS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UUserWidget>(this, MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
			UE_LOG(LogPITS, Log, TEXT("'%s' Created Main Menu Widget '%s'"), *GetNameSafe(this), *GetNameSafe(MainWidget));
		}
	}
	UpdateMainWidget();
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
			for (const UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
				UE_LOG(LogPITS, Log, TEXT("'%s' Added Input Mapping Context '%s'"), *GetNameSafe(this), *GetNameSafe(CurrentContext));
			}
		}
	}
}

void APITS_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UpdateMainWidget();
}

void APITS_PlayerController::UpdateMainWidget()
{
	APawn* NewPawn = GetPawn();
	if (!NewPawn) return;
	
	// cast to character
	const APITS_BasePlayerCharacter* PossessedCharacter = Cast<APITS_BasePlayerCharacter>(NewPawn);
	if (!PossessedCharacter)
	{
		UE_LOG(LogPITS, Warning, TEXT("'%s' OnPossess(): Possessed Pawn '%s' is not a APITS_BasePlayerCharacter"), *GetNameSafe(this), *GetNameSafe(NewPawn));
		return;
	}
	
	if (MainWidget && MainWidget->GetClass()->ImplementsInterface(UPITS_StatsWidgetInterface::StaticClass()))
	{
		IPITS_StatsWidgetInterface::Execute_UpdateName(MainWidget, PossessedCharacter->GetCharacterName());
		IPITS_StatsWidgetInterface::Execute_UpdateDescription(MainWidget, PossessedCharacter->GetCharacterDescription());
		IPITS_StatsWidgetInterface::Execute_UpdateIcon(MainWidget, PossessedCharacter->GetCharacterIcon());
	}
}

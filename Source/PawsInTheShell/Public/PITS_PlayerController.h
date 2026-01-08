// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "PITS_PlayerController.generated.h"

class APITS_BasePlayerCharacter;
/**
 * 
 */
UCLASS(Abstract, Config="Game")
class PAWSINTHESHELL_API APITS_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** Main Viewport Widget Instance */
	UPROPERTY()
	UUserWidget* MainWidget;
	
	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|InputMappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Main Menu Widget Class to show character info */
	UPROPERTY(EditAnywhere, Category="PawsInTheShell|UI")
	TSubclassOf<UUserWidget> MainWidgetClass;
	
	UFUNCTION()
	void UpdateMainWidget();
	
	virtual void BeginPlay() override;
	
	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
	
	virtual void OnPossess(APawn* InPawn) override;
};

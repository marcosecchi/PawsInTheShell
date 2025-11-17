// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PITS_CharacterDataTableRow.generated.h"

/**
 *  Holds information about character stats and properties
 */
USTRUCT(BlueprintType, meta=(DisplayName="CharacterDataTableRow"))
struct FPITS_CharacterDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(ToolTip="Name of the character displayed in game"))
	FText CharacterName;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Character description (i.e.: speciality)"))
	FText CharacterDescription;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Character icon"))
	UTexture2D* CharacterIcon;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Maximum health reachable by the character"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Character health at the start of the game"))
	float StartingHealth = 100.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Character defensive amount that reduces incoming damage"))
	float ArmourAmount = 0.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Can the character regenerate when in a regeneration zone?"))
	bool bCanRegenerate = false;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Maximum speed of the character when moving"))
	float MaxWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Initial velocity (instantaneous force acceleration) applied when the character jumps"))
	float JumpZVelocity = 500.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Amount of lateral control for the character while in air (0 = no control, 1 = full control (Max Walk Speed)"))
	float AirControl = 0.35f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Deceleration applied when walking and not applying acceleration"))
	float BrakingDecelerationWalking = 2000.f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="LAteral deceleration applied when falling and not applying acceleration"))
	float BrakingDecelerationFalling = 1500.0f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Additional notes about this character (for development purposes)"))
	FString Notes;
};

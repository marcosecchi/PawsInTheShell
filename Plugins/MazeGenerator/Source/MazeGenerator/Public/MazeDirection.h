#pragma once

#include "CoreMinimal.h"

/* **
 * Used to represent the four main directions in a maze during generation.
 */
UENUM(BlueprintType, meta=(DisplayName="MazeDirection"))
enum class EMazeDirection: uint8
{
	North,
	East,
	South,
	West
};

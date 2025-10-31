// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "PITS_PlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PAWSINTHESHELL_API APITS_PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	APITS_PlayerCameraManager();
};

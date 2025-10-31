// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PawsInTheShell/Public/PITS_PlayerCameraManager.h"

APITS_PlayerCameraManager::APITS_PlayerCameraManager()
{
	// set the min/max pitch
	ViewPitchMin = -70.0f;
	ViewPitchMax = 80.0f;
}

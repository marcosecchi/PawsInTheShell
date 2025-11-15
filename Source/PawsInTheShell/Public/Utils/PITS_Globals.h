// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PITS_Globals.generated.h"

#define PITS_PROJECT_VERSION "alpha 1.0.0"

/**
 * Global constants and functions
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_Globals : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:

public:
	UFUNCTION(BlueprintPure, Category="Paws In The Shell|Globals")
	static FString GetProjectVersion() { return PITS_PROJECT_VERSION;}


#pragma region Tags
private:
	static constexpr const TCHAR* PlayerTag = TEXT("Player");
	static constexpr const TCHAR* EnemyTag = TEXT("Enemy");
	static constexpr const TCHAR* BulletTag = TEXT("Bullet");
	static constexpr const TCHAR* PickupTag = TEXT("Pickup");

public:
	UFUNCTION(BlueprintPure, Category="Paws In The Shell|Globals")
	static FName GetPlayerTag() { return PlayerTag;}

	UFUNCTION(BlueprintPure, Category="Paws In The Shell|Globals")
	static FName GetEnemyTag() { return EnemyTag;}

	UFUNCTION(BlueprintPure, Category="Paws In The Shell|Globals")
	static FName GetBulletTag() { return BulletTag;}

	UFUNCTION(BlueprintPure, Category="Paws In The Shell|Globals")
	static FName GetPickupTag() { return PickupTag;}
#pragma endregion
	
};

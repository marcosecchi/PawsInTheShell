// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PITS_Globals.generated.h"

/**
 * Global constants and functions
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_Globals : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	inline static const FString ProjectVersion = TEXT("Alpha 0.1");

public:
	UFUNCTION(BlueprintPure, Category="Paws In The Shell|Globals")
	static FString GetProjectVersion() { return ProjectVersion;}


#pragma region Tags
private:
	inline static const FName PlayerTag = FName("Player");
	inline static const FName EnemyTag = FName("Enemy");

public:
	UFUNCTION(BlueprintPure, Category="Paws In The Shell|Globals")
	static FName GetPlayerTag() { return PlayerTag;}

	UFUNCTION(BlueprintPure, Category="Paws In The Shell|Globals")
	static FName GetEnemyTag() { return EnemyTag;}
#pragma endregion
	
};

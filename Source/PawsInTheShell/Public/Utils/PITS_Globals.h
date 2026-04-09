// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PITS_Globals.generated.h"

#define PITS_PROJECT_VERSION "alpha 11.0.0"

#define PITS_LOG_PROJECT_VERSION UE_LOG(LogTemp, Log, TEXT("Paws In The Shell - Project Version: %s"), TEXT(PITS_PROJECT_VERSION))

// Checks a pointer and logs a warning if it is null.
#define CHECK_PTR_AND_LOG(Ptr)                                                 \
	do                                                                         \
	{                                                                          \
		if ((Ptr) == nullptr)                                                  \
		{                                                                      \
			UE_LOG(LogPITS, Warning, TEXT("Null pointer detected: %s [%s:%d]"),  \
				   TEXT(#Ptr), TEXT(__FILE__), __LINE__);                      \
		}                                                                      \
	} while (0)

// Checks a pointer, logs a warning if null and returns.
#define CHECK_PTR_AND_LOG_RETURN(Ptr)                                          \
	do                                                                         \
	{                                                                          \
		if ((Ptr) == nullptr)                                                  \
		{                                                                      \
			UE_LOG(LogPITS, Warning, TEXT("Null pointer detected: %s [%s:%d]"),  \
				   TEXT(#Ptr), TEXT(__FILE__), __LINE__);                      \
			return;                                                            \
		}                                                                      \
	} while (0)

// Checks a pointer, logs a warning if null, and assigns a fallback value to the pointer.
#define CHECK_PTR_WITH_FALLBACK(Ptr, FallbackValue)                            \
	do                                                                         \
	{                                                                          \
		if ((Ptr) == nullptr)                                                  \
		{                                                                      \
			UE_LOG(LogPITS, Warning, TEXT("Null pointer detected: %s [%s:%d]"),  \
				   TEXT(#Ptr), TEXT(__FILE__), __LINE__);                      \
			(Ptr) = (FallbackValue);                                           \
		}                                                                      \
	} while (0)

/**
 * Global constants and functions
 */
UCLASS()
class PAWSINTHESHELL_API UPITS_Globals : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:

public:
	UFUNCTION(BlueprintPure, Category="PawsInTheShell|Globals")
	static FString GetProjectVersion() { return PITS_PROJECT_VERSION;}


#pragma region Tags
private:
	static constexpr const TCHAR* PlayerTag = TEXT("Player");
	static constexpr const TCHAR* EnemyTag = TEXT("Enemy");
	static constexpr const TCHAR* BulletTag = TEXT("Bullet");
	static constexpr const TCHAR* PickupTag = TEXT("Pickup");

public:
	UFUNCTION(BlueprintPure, Category="PawsInTheShell|Globals")
	static FName GetPlayerTag() { return PlayerTag;}

	UFUNCTION(BlueprintPure, Category="PawsInTheShell|Globals")
	static FName GetEnemyTag() { return EnemyTag;}

	UFUNCTION(BlueprintPure, Category="PawsInTheShell|Globals")
	static FName GetBulletTag() { return BulletTag;}

	UFUNCTION(BlueprintPure, Category="PawsInTheShell|Globals")
	static FName GetPickupTag() { return PickupTag;}
#pragma endregion
	
};

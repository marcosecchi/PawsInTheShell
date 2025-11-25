// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "PITS_Globals.generated.h"

// Checks a pointer and logs a warning if it is null.
#define CHECK_PTR_AND_LOG(Ptr)                                                 \
	do                                                                         \
	{                                                                          \
		if ((Ptr) == nullptr)                                                  \
		{                                                                      \
			UE_LOG(LogGF, Warning, TEXT("Null pointer detected: %s [%s:%d]"),  \
				   TEXT(#Ptr), TEXT(__FILE__), __LINE__);                      \
		}                                                                      \
	} while (0)

// Checks a pointer, logs a warning if null and returns.
#define CHECK_PTR_AND_LOG_RETURN(Ptr)                                          \
	do                                                                         \
	{                                                                          \
		if ((Ptr) == nullptr)                                                  \
		{                                                                      \
			UE_LOG(LogGF, Warning, TEXT("Null pointer detected: %s [%s:%d]"),  \
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
			UE_LOG(LogGF, Warning, TEXT("Null pointer detected: %s [%s:%d]"),  \
				   TEXT(#Ptr), TEXT(__FILE__), __LINE__);                      \
			(Ptr) = (FallbackValue);                                           \
		}                                                                      \
	} while (0)

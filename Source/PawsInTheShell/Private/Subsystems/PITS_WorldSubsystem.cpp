// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Subsystems/PITS_WorldSubsystem.h"

#include "NavigationSystem.h"
#include "PITS_BasePlayerCharacter.h"
#include "Engine/TriggerVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/PITS_Logs.h"
#include "Utils/PITS_Globals.h"

void UPITS_WorldSubsystem::ChangeCharacter() const
{
	UE_LOG(LogPITS, Log, TEXT("UPITS_WorldSubsystem::ChangeCharacter()"));
	
	// Get all characters in the level
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APITS_BasePlayerCharacter::StaticClass(), FoundCharacters);
	
	// Get a random character from the list
	if (FoundCharacters.Num() > 1)
	{
		// Get character controller
		if (AController* Controller = GetWorld()->GetFirstPlayerController())
		{
			// Remove the currently possessed character from the list
			FoundCharacters.Remove(Controller->GetPawn());

			const int32 RandomIndex = FMath::RandRange(0, FoundCharacters.Num() - 1);
			if (APITS_BasePlayerCharacter* NewCharacter = Cast<APITS_BasePlayerCharacter>(FoundCharacters[RandomIndex]))
			{
				Controller->Possess(NewCharacter);
				NotifyUpdateWeapon();
				UE_LOG(LogPITS, Log, TEXT("'%s' Possessed Character '%s'"), *GetNameSafe(this), *GetNameSafe(NewCharacter));
			}
		}
	}
}

#pragma region "Damage"

void UPITS_WorldSubsystem::NotifyDamageTaken(const TSubclassOf<UDamageType>& DamageType, const float Amount, APawn* DamagedPawn) const
{
	OnCharacterDamageTaken.Broadcast(DamageType, Amount, DamagedPawn);
}

void UPITS_WorldSubsystem::NotifyDamageEnd(const TSubclassOf<UDamageType>& DamageType) const
{
	OnCharacterDamageEnd.Broadcast(DamageType);
}

void UPITS_WorldSubsystem::NotifyUpdateWeapon() const
{
	OnCharacterUpdateWeapon.Broadcast();
}
#pragma endregion
	
#pragma region "Spawners"

void UPITS_WorldSubsystem::SpawnActorsOnGameThread(UWorld* World, const TSubclassOf<AActor> SpawnableActorClass, TArray<FVector> SpawnLocations)
{
	UE_LOG(LogPITS, Log, TEXT("Spawning %d actors on the Game Thread..."), SpawnLocations.Num());
	for (const FVector& Location : SpawnLocations)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		World->SpawnActor<AActor>(SpawnableActorClass, Location, FRotator::ZeroRotator, SpawnParams);
	}
}

// Asynchronously compute a set of random spawn locations inside the bounds of a TriggerVolume,
// validate those locations on the Game Thread with line traces, and finally spawn actors there.
void UPITS_WorldSubsystem::SpawnActorsAsync(const TSubclassOf<AActor> SpawnableActorClass, const int32 MinAmount, const int32 MaxAmount, ATriggerVolume* SpawnArea)
{
    UWorld* World = GetWorld();
    CHECK_PTR_AND_LOG_RETURN(World);                // Ensure the world exists
    CHECK_PTR_AND_LOG_RETURN(SpawnableActorClass);  // Ensure a valid actor class was provided

    // Get a plain-data bounding box for the SpawnArea. We compute this on the Game Thread
    // so the async lambda doesn't need to touch the SpawnArea UObject.
    FBox SpawnBox = SpawnArea->GetComponentsBoundingBox(false);
    
    // Launch heavy/CPU-bound computations on a ThreadPool thread. The lambda captures only
    // plain data (World pointer, SpawnableActorClass, SpawnBox, and integer amounts).
    // NOTE: we capture by value to avoid accessing UObjects directly on the worker thread.
    Async(EAsyncExecution::ThreadPool, [World, SpawnableActorClass, SpawnBox, MinAmount, MaxAmount]()
    {
        UE_LOG(LogPITS, Log, TEXT("Computing spawn locations asynchronously..."));
        
        // Clamp min/max to sane values and pick how many actors to attempt to spawn.
        const int32 ClampedMin = FMath::Max(0, MinAmount);
        const int32 ClampedMax = FMath::Max(ClampedMin, MaxAmount);
        const int32 SpawnCount = FMath::RandRange(ClampedMin, ClampedMax);
        
        TArray<FVector> SpawnLocations;
        UE_LOG(LogPITS, Log, TEXT("Calculated %d potential spawn locations..."), SpawnCount);

        // Compute a set of approximate spawn positions inside the box.
        // IMPORTANT: This loop does NOT call any UObject APIs or do physics traces;
        // it only uses plain FVector and FMath so it's safe to run on a worker thread.
        for (int32 i = 0; i < SpawnCount; ++i)
        {
            // Pick random X/Y inside the box, start Z above the box so we can trace down later.
            FVector Location(
              FMath::FRandRange(SpawnBox.Min.X, SpawnBox.Max.X),
              FMath::FRandRange(SpawnBox.Min.Y, SpawnBox.Max.Y),
              SpawnBox.Max.Z + 100.0f // Start above the box to trace down later
            );
            SpawnLocations.Add(Location);
        }

        UE_LOG(LogPITS, Log, TEXT("Finished computing spawn locations asynchronously. Now validating on Game Thread..."));

        // Switch back to the Game Thread to perform any engine/physics/UObject work (line traces and spawning).
        // We capture the plain SpawnLocations array and the World pointer by value here.
        AsyncTask(ENamedThreads::GameThread, [World = World, SpawnableActorClass, SpawnLocations, SpawnBox]()
        {
            UE_LOG(LogPITS, Log, TEXT("Validating spawn locations on Game Thread..."));

            TArray<FVector> ValidatedLocations;

            // For each candidate location, perform a line trace down to find a valid surface point.
            // This must run on the Game Thread because it calls UWorld::LineTraceSingleByChannel.
            for (const FVector Location : SpawnLocations)
            {
                FHitResult HitResult;
                FVector TraceStart = Location;
                FVector TraceEnd(TraceStart.X, TraceStart.Y, SpawnBox.Min.Z); // trace down to the bottom of the box
                
                // Perform a visibility trace. If we hit something, take that hit location as valid.
                if (const bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility))
                {
                    ValidatedLocations.Add(HitResult.Location);
                }
            }

            UE_LOG(LogPITS, Log, TEXT("Validated %d spawn locations. Now spawning actors on Game Thread..."), ValidatedLocations.Num());

            // Spawn the actors at validated locations (still on Game Thread).
            SpawnActorsOnGameThread(World, SpawnableActorClass, ValidatedLocations);
        });
    });
}


void UPITS_WorldSubsystem::SpawnActorsParallel(TSubclassOf<AActor> SpawnableActorClass, const int32 MinAmount,
	const int32 MaxAmount, const FVector SpawnCenterLocation, const float SpawnRadius)
{
	const int32 ClampedMin = FMath::Max(0, MinAmount);
	const int32 ClampedMax = FMath::Max(ClampedMin, MaxAmount);
	const int32 SpawnCount = FMath::RandRange(ClampedMin, ClampedMax);

	// IMPORTANT NOTE: This method is not truly parallel and may cause performance issues if SpawnCount is large,
	// as it spawns actors immediately on the game thread.
	// For true parallel spawning, consider batching spawn requests and processing them in a more optimized way

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		UWorld* World = GetWorld();
		CHECK_PTR_AND_LOG_RETURN(World);
		
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
		CHECK_PTR_AND_LOG_RETURN(NavSys);

		FVector2D RandomOffset = FMath::RandPointInCircle(SpawnRadius);
		FVector RandomLocation = SpawnCenterLocation + FVector(RandomOffset.X, RandomOffset.Y, 0.f);

		FVector Extent = FVector(200.f, 200.f, 100.f);
		if (FNavLocation NavLocation; NavSys->ProjectPointToNavigation(RandomLocation, NavLocation, Extent))
		{
			FVector SpawnLocation = NavLocation.Location;
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			World->SpawnActor<AActor>(SpawnableActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			UE_LOG(LogPITS, Log, TEXT("Spawned actor at location %s"), *SpawnLocation.ToString());
		}
	}
}

#pragma endregion 

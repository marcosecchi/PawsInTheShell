// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Subsystems/PITS_WorldSubsystem.h"

#include "EngineUtils.h"
#include "PITS_BasePlayerCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "Interfaces/PITS_PooledObjectInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/PITS_Logs.h"
#include "Utils/PITS_FixedActorPool.h"
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

void UPITS_WorldSubsystem::SpawnActorsAsync(const TSubclassOf<AActor> SpawnableActorClass, const int32 MinAmount, const int32 MaxAmount)
{
	// On game thread: gather actor bounds & combined bounding box
	FBox CombinedBox(EForceInit::ForceInit);
	UWorld* World = GetWorld();
	CHECK_PTR_AND_LOG_RETURN(World);
	CHECK_PTR_AND_LOG_RETURN(SpawnableActorClass);
	if (!World || !SpawnableActorClass) return;

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (const AActor* Actor = *It)
	    {
	        constexpr bool bOnlyCollidingComponents = false;
	        FBox BoundingBox = Actor->GetComponentsBoundingBox(bOnlyCollidingComponents);
	        CombinedBox += BoundingBox;
	    }
	}

	// Move heavy computation to async thread with only plain data
	Async(EAsyncExecution::ThreadPool, [World, SpawnableActorClass, CombinedBox, MinAmount, MaxAmount]()
	{
	    // Heavy computation (spawn count, random locations in CombinedBox)
	    const int32 ClampedMin = FMath::Max(0, MinAmount);
	    const int32 ClampedMax = FMath::Max(ClampedMin, MaxAmount);
	    const int32 SpawnCount = FMath::RandRange(ClampedMin, ClampedMax);
	    
	    TArray<FVector> SpawnLocations;
	    // Async: Just compute approximate locations heuristically, no UObject calls here
	    for (int32 i = 0; i < SpawnCount; ++i)
	    {
	        // Compute random spawn location inside CombinedBox bounds but no trace or physics here
	        FVector Location(
	          FMath::FRandRange(CombinedBox.Min.X, CombinedBox.Max.X),
	          FMath::FRandRange(CombinedBox.Min.Y, CombinedBox.Max.Y),
	          CombinedBox.Max.Z + 100.0f // Temporarily high Z; adjust later on game thread
	        );
	        SpawnLocations.Add(Location);
	    }

	    // Schedule actual line traces and final location validation on game thread
	    AsyncTask(ENamedThreads::GameThread, [World = World, SpawnableActorClass, SpawnLocations, CombinedBox]()
	    {
	        TArray<FVector> ValidatedLocations;
	        for (FVector Location : SpawnLocations)
	        {
	            FHitResult HitResult;
	            FVector TraceStart = Location;
	            FVector TraceEnd(TraceStart.X, TraceStart.Y, CombinedBox.Min.Z); // Trace down

	            if (const bool bHit = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility))
	            {
	                ValidatedLocations.Add(HitResult.Location);
	            }
	        }
	        SpawnActorsOnGameThread(World, SpawnableActorClass, ValidatedLocations);
	    });
	});
}
 
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
#pragma endregion 

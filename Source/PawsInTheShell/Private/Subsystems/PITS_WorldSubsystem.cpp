// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Subsystems/PITS_WorldSubsystem.h"
#include "PITS_BasePlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/PITS_Logs.h"
#include "Utils/PITS_ActorPool.h"
#include "Utils/PITS_Globals.h"

void UPITS_WorldSubsystem::ChangeCharacter()
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
				UE_LOG(LogPITS, Log, TEXT("'%s' Possessed Character '%s'"), *GetNameSafe(this), *GetNameSafe(NewCharacter));
			}
		}
	}
}

void UPITS_WorldSubsystem::NotifyDamageTaken(const TSubclassOf<UDamageType>& DamageType, const float Amount)
{
	OnDamageTaken.Broadcast(DamageType, Amount);
}

void UPITS_WorldSubsystem::NotifyDamageEnd(const TSubclassOf<UDamageType>& DamageType)
{
	OnDamageEnd.Broadcast(DamageType);
}

void UPITS_WorldSubsystem::CreateObjectPool(const TSubclassOf<AActor> SpawnableClass, const int32 PoolSize)
{
	if (!SpawnableClass)
	{
		UE_LOG(LogPITS, Warning, TEXT("CreateObjectPool called with null SpawnableClass"));
		return;
	}

	if (HasObjectPool(SpawnableClass))
	{
		UE_LOG(LogPITS, Warning, TEXT("Object pool for class %s already exists"), *GetNameSafe(SpawnableClass));
		return;
	}

	if (UPITS_ActorPool* NewPool = NewObject<UPITS_ActorPool>(this))
	{
		NewPool->InitializePool(SpawnableClass, PoolSize);
		ActorsPoolMap.Add(SpawnableClass, NewPool);
		UE_LOG(LogPITS, Log, TEXT("Created object pool for class %s with size %d"), *GetNameSafe(SpawnableClass), PoolSize);
	}
}

bool UPITS_WorldSubsystem::HasObjectPool(const TSubclassOf<AActor> SpawnableClass) const
{
	return ActorsPoolMap.Contains(SpawnableClass);
}

AActor* UPITS_WorldSubsystem::GetPooledObject(const TSubclassOf<AActor> SpawnableClass)
{
	// Ensure the pool exists
	if (!HasObjectPool(SpawnableClass)) CreateObjectPool(SpawnableClass);
	return GetObjectPool(SpawnableClass)->GetObjectFromPool();
}

void UPITS_WorldSubsystem::ReleasePooledObject(AActor* Actor)
{
	CHECK_PTR_AND_LOG_RETURN(Actor);
	if (const TSubclassOf<AActor> ActorClass = Actor->GetClass(); HasObjectPool(ActorClass))
	{
		GetObjectPool(ActorClass)->ReleaseObjectToPool(Actor);
	}
	else
	{
		UE_LOG(LogPITS, Warning, TEXT("No object pool found for class %s when releasing actor %s"), *GetNameSafe(ActorClass), *GetNameSafe(Actor));
	}
}

UPITS_ActorPool* UPITS_WorldSubsystem::GetObjectPool(const TSubclassOf<AActor> SpawnableClass) const
{
	return ActorsPoolMap.FindRef(SpawnableClass);
}

bool UPITS_WorldSubsystem::HasAvailablePooledObjects(const TSubclassOf<AActor> SpawnableClass) const
{
	if (!HasObjectPool(SpawnableClass))
	{
		UE_LOG(LogPITS, Warning, TEXT("No object pool found for class %s"), *GetNameSafe(SpawnableClass));
		return false;
	}
	return GetObjectPool(SpawnableClass)->HasAvailableObjectsInPool();
}

bool UPITS_WorldSubsystem::IsObjectPooled(const AActor* Actor) const
{
	if (const TSubclassOf<AActor> Class = Actor->GetClass(); HasObjectPool(Class))
	{
		const UPITS_ActorPool* Pool = GetObjectPool(Class);
		return Pool->IsObjectPooled(Actor);
	}
	return false;
}

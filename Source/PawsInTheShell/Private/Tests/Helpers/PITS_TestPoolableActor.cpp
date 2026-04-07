// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "Tests/Helpers/PITS_TestPoolableActor.h"
#include "Utils/PITS_Logs.h"

APITS_TestPoolableActor::APITS_TestPoolableActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APITS_TestPoolableActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(true);
}

void APITS_TestPoolableActor::HandleAcquire_Implementation()
{
	SetActorHiddenInGame(false);

	UE_LOG(LogPITS, Log, TEXT("'%s' Acquired from pool."), *GetNameSafe(this));
}

void APITS_TestPoolableActor::HandleRelease_Implementation()
{
	SetActorHiddenInGame(true);

	UE_LOG(LogPITS, Log, TEXT("'%s' Released to pool."), *GetNameSafe(this));
}

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_SafeZone.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/DefenceInterface.h"
#include "Interfaces/HealthInterface.h"
#include "Utils/PITS_Logs.h"

void APITS_SafeZone::HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

	// Check if actor implements the Defence Interface
	if (OtherActor->GetClass()->ImplementsInterface(UDefenceInterface::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Actor '%s' entered a safe zone"), *GetNameSafe(this), *GetNameSafe(OtherActor));
		IDefenceInterface::Execute_SetIsInSafeZone(OtherActor, true);
	}
}

void APITS_SafeZone::HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Check if actor implements the Defence Interface
	if (OtherActor->GetClass()->ImplementsInterface(UDefenceInterface::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Actor '%s' exited a safe zone"), *GetNameSafe(this), *GetNameSafe(OtherActor));
		IDefenceInterface::Execute_SetIsInSafeZone(OtherActor, false);
	}
}

// Sets default values
APITS_SafeZone::APITS_SafeZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetArrowColor(FLinearColor(0.0f, 1.0f, 1.0f));
	Arrow->SetArrowLength(160.0f);

	SafeZoneVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SafeZoneVolume"));
	SafeZoneVolume->SetupAttachment(RootComponent);
	SafeZoneVolume->SetBoxExtent(FVector(150.0f, 150.0f, 150.0f));
	OnActorBeginOverlap.AddDynamic(this, &APITS_SafeZone::HandleActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &APITS_SafeZone::HandleActorEndOverlap);
}


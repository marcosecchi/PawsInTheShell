// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "PITS_SafeZone.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PITS_DefenceInterface.h"
#include "Interfaces/PITS_SafeZoneInterface.h"
#include "Utils/PITS_Logs.h"

void APITS_SafeZone::HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

	// Check if actor implements the Defence Interface
	if (OtherActor->GetClass()->ImplementsInterface(UPITS_DefenceInterface::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Actor '%s' entered a safe zone"), *GetNameSafe(this), *GetNameSafe(OtherActor));
		IPITS_SafeZoneInterface::Execute_SetIsInSafeZone(OtherActor, true);
	}
}

void APITS_SafeZone::HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Check if actor implements the Defence Interface
	if (OtherActor->GetClass()->ImplementsInterface(UPITS_DefenceInterface::StaticClass()))
	{
		UE_LOG(LogPITS, Log, TEXT("'%s' Actor '%s' exited a safe zone"), *GetNameSafe(this), *GetNameSafe(OtherActor));
		IPITS_SafeZoneInterface::Execute_SetIsInSafeZone(OtherActor, false);
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

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SafeZoneVolume = CreateDefaultSubobject<USphereComponent>(TEXT("SafeZoneVolume"));
	SafeZoneVolume->SetupAttachment(RootComponent);
	SafeZoneVolume->SetSphereRadius(500.0f);
	OnActorBeginOverlap.AddDynamic(this, &APITS_SafeZone::HandleActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &APITS_SafeZone::HandleActorEndOverlap);
}


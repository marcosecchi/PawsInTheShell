// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "PITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest.generated.h"

UCLASS()
class PAWSINTHESHELL_API APITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest : public AFunctionalTest
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APITS_ObjectPoolSubsystem_AcquireObject_FunctionalTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "WeaponSpawnPointComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=("Paws In The Shell"), meta=(BlueprintSpawnableComponent))
class PAWSINTHESHELL_API UWeaponSpawnPointComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponSpawnPointComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
};

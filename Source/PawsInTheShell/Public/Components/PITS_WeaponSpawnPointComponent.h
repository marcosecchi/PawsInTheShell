// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "PITS_WeaponSpawnPointComponent.generated.h"

class APITS_BaseProjectile;

UCLASS(Blueprintable, BlueprintType, ClassGroup=("PawsInTheShell"), meta=(BlueprintSpawnableComponent))
class PAWSINTHESHELL_API UPITS_WeaponSpawnPointComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPITS_WeaponSpawnPointComponent();

protected:
	UPROPERTY()
	TSubclassOf<APITS_BaseProjectile> CurrentProjectileClass;
	
public:

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Shoot();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetCurrentProjectileClass(const TSubclassOf<APITS_BaseProjectile> NewProjectileClass);
};

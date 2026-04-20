// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PITS_ArmourComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PAWSINTHESHELL_API UPITS_ArmourComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	float ArmourAmount = 0.f;

	UPROPERTY()
	bool bIsCybernetic = false;

public:
	UPITS_ArmourComponent();

	UFUNCTION()
	void SetArmourAmount(const float NewArmourAmount) {	ArmourAmount = FMath::Max(0.f, NewArmourAmount); }

	UFUNCTION()
	float GetArmourAmount() const { return ArmourAmount; }
	
	UFUNCTION()
	void SetIsCybernetic(const bool bNewIsCybernetic) { bIsCybernetic =  bNewIsCybernetic; }
	
	UFUNCTION()
	bool IsCybernetic() const { return bIsCybernetic; }
	
	UFUNCTION()
	float GetActualDamage(const float IncomingDamage, const UClass* DamageTypeClass) const;
};

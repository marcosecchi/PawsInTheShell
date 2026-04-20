// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "CQTest.h"
#include "Components/PITS_ArmourComponent.h"
#include "Damage/PITS_DamageType_ArmourPiercing.h"
#include "Damage/PITS_DamageType_CyberTech.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

TEST_CLASS(PITS_ArmourComponent, "PawsInTheShell.Components")
{
	
	AActor* TestActor = nullptr;
	UPITS_ArmourComponent* ArmourComponent = nullptr;

	BEFORE_ALL()
	{}
	
	BEFORE_EACH()
	{
		UWorld* World = GWorld;
		TestActor = World->SpawnActor<AActor>();
		ArmourComponent = NewObject<UPITS_ArmourComponent>(TestActor);
		TestActor->AddComponent(TEXT("ArmourComponent"), false, FTransform::Identity, ArmourComponent);
	}

	AFTER_EACH()
	{
		TestActor->Destroy();
	}

	AFTER_ALL()
	{}

	TEST_METHOD(SetArmourAmount)
	{
		ArmourComponent->SetArmourAmount(50.f);
		ASSERT_THAT(IsTrue(ArmourComponent->GetArmourAmount() == 50.f));
	}

	TEST_METHOD(SetIsCybernetic_True)
	{
		ArmourComponent->SetIsCybernetic(true);
		ASSERT_THAT(IsTrue(ArmourComponent->IsCybernetic()));
	}

	TEST_METHOD(SetIsCybernetic_False)
	{
		ArmourComponent->SetIsCybernetic(false);
		ASSERT_THAT(IsTrue(!ArmourComponent->IsCybernetic()));
	}

	TEST_METHOD(GetActualDamage_PartialAbsorption)
	{
		ArmourComponent->SetArmourAmount(10.f);
		const float ActualDamage = ArmourComponent->GetActualDamage(50.f, UDamageType::StaticClass());
		ASSERT_THAT(IsTrue(ActualDamage == 40.0f));
	}

	TEST_METHOD(GetActualDamage_FullAbsorption)
	{
		ArmourComponent->SetArmourAmount(60.f);
		const float ActualDamage = ArmourComponent->GetActualDamage(50.f, UDamageType::StaticClass());
		ASSERT_THAT(IsTrue(ActualDamage == 0.0f));
	}

	TEST_METHOD(GetActualDamage_CyberTechOnCybernetic)
	{
		ArmourComponent->SetArmourAmount(0.f);
		ArmourComponent->SetIsCybernetic(true);
		const float ActualDamage = ArmourComponent->GetActualDamage(100.f, UPITS_DamageType_CyberTech::StaticClass());
		ASSERT_THAT(IsTrue(ActualDamage == 100.0f));
	}

	TEST_METHOD(GetActualDamage_CyberTechOnCybernetic_WithArmour)
	{
		ArmourComponent->SetArmourAmount(50.f);
		ArmourComponent->SetIsCybernetic(true);
		const float ActualDamage = ArmourComponent->GetActualDamage(100.f, UPITS_DamageType_CyberTech::StaticClass());
		ASSERT_THAT(IsTrue(ActualDamage == 100.0f)); // Cybertech damage ignores armour
	}

	TEST_METHOD(GetActualDamage_CyberTechOnOrganic)
	{
		ArmourComponent->SetArmourAmount(0.f);
		ArmourComponent->SetIsCybernetic(false);
		const float ActualDamage = ArmourComponent->GetActualDamage(100.f, UPITS_DamageType_CyberTech::StaticClass());
		ASSERT_THAT(IsTrue(ActualDamage == 0.0f));
	}
	
	TEST_METHOD(GetActualDamage_ArmourPiercing)
	{
		ArmourComponent->SetArmourAmount(1000.f);
		const float ActualDamage = ArmourComponent->GetActualDamage(100.f, UPITS_DamageType_ArmourPiercing::StaticClass());
		ASSERT_THAT(IsTrue(ActualDamage == 100.0f));
	}

};
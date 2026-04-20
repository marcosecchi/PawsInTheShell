// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "CQTest.h"
#include "Components/PITS_HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

TEST_CLASS(PITS_HealthComponent, "PawsInTheShell.Components")
{
	
	AActor* TestActor = nullptr;
	UPITS_HealthComponent* HealthComponent = nullptr;

	BEFORE_ALL()
	{}
	
	BEFORE_EACH()
	{
		UWorld* World = GWorld;
		TestActor = World->SpawnActor<AActor>();
		HealthComponent = NewObject<UPITS_HealthComponent>(TestActor);
		TestActor->AddComponent(TEXT("HealthComponent"), false, FTransform::Identity, HealthComponent);
	}

	AFTER_EACH()
	{
		TestActor->Destroy();
	}

	AFTER_ALL()
	{}

	TEST_METHOD(SetHealth_Full)
	{
		HealthComponent->SetMaxHealth(100.0f);
		HealthComponent->SetCurrentHealth(100.0f);
		ASSERT_THAT(IsTrue(HealthComponent->GetHealthPercentage() == 1.0f));
	}

	TEST_METHOD(SetHealth_Zero)
	{
		HealthComponent->SetMaxHealth(100.0f);
		HealthComponent->SetCurrentHealth(0.0f);
		ASSERT_THAT(IsTrue(HealthComponent->GetHealthPercentage() == 0.0f));
	}
	
	TEST_METHOD(SetHealth_Half)
	{
		HealthComponent->SetMaxHealth(100.0f);
		HealthComponent->SetCurrentHealth(50.0f);
		ASSERT_THAT(IsTrue(HealthComponent->GetHealthPercentage() == 0.5f));
	}

	TEST_METHOD(SetHealth_ClampMax)
	{
		HealthComponent->SetMaxHealth(100.0f);
		HealthComponent->SetCurrentHealth(300.0f);
		ASSERT_THAT(IsTrue(HealthComponent->GetHealthPercentage() == 1.0f));
	}

	TEST_METHOD(Add)
	{
		HealthComponent->SetMaxHealth(100.0f);
		HealthComponent->SetCurrentHealth(10.0f);
		HealthComponent->AddHealth(40.0f);
		ASSERT_THAT(IsTrue(HealthComponent->GetHealthPercentage() == 0.5f));
	}

	TEST_METHOD(Add_ClampMax)
	{
		HealthComponent->SetMaxHealth(100.0f);
		HealthComponent->SetCurrentHealth(50.0f);
		HealthComponent->AddHealth(100.0f);
		ASSERT_THAT(IsTrue(HealthComponent->GetHealthPercentage() == 1.0f));
	}

	TEST_METHOD(Remove_Full)
	{
		HealthComponent->SetMaxHealth(100.0f);
		HealthComponent->SetCurrentHealth(100.0f);
		HealthComponent->RemoveHealth(100.0f);
		ASSERT_THAT(IsTrue(HealthComponent->GetHealthPercentage() == 0.0f));
	}

	TEST_METHOD(Remove_Half)
	{
		HealthComponent->SetMaxHealth(100.0f);
		HealthComponent->SetCurrentHealth(100.0f);
		HealthComponent->RemoveHealth(50.0f);
		ASSERT_THAT(IsTrue(HealthComponent->GetHealthPercentage() == 0.5f));
	}

	TEST_METHOD(CanRegenerate_SetTrue)
	{
		HealthComponent->SetCanRegenerate(true);
		ASSERT_THAT(IsTrue(HealthComponent->CanRegenerate()));
	}

	TEST_METHOD(CanRegenerate_SetFalse)
	{
		HealthComponent->SetCanRegenerate(false);
		ASSERT_THAT(IsTrue(!HealthComponent->CanRegenerate()));
	}

	TEST_METHOD(IsDead_AfterDamage)
	{
		HealthComponent->SetMaxHealth(100.0f);
		HealthComponent->SetCurrentHealth(100.0f);
		HealthComponent->RemoveHealth(150.0f);
		ASSERT_THAT(IsTrue(HealthComponent->IsDead()));
	}

	TEST_METHOD(IsAlive_AfterDamage)
	{
		HealthComponent->SetMaxHealth(100.0f);
		HealthComponent->SetCurrentHealth(100.0f);
		HealthComponent->RemoveHealth(50.0f);
		ASSERT_THAT(IsTrue(!HealthComponent->IsDead()));
	}

};
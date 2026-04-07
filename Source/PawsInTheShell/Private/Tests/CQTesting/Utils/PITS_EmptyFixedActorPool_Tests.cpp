// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "CQTest.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Utils/PITS_FixedActorPool.h"

TEST_CLASS(PITS_EmptyFixedActorPool, "PawsInTheShell.Utils")
{
    UPITS_FixedActorPool* TestPool = nullptr;
	int32 PoolSize = 0;
	
	BEFORE_ALL()
	{}
	
	BEFORE_EACH()
	{
		UWorld* World = GWorld;
    	TestPool = NewObject<UPITS_FixedActorPool>(World);
	}

	AFTER_EACH()
	{
        if (TestPool)
        {
            TestPool->ConditionalBeginDestroy();
            TestPool = nullptr;
        }
    }

	AFTER_ALL()
	{}

	TEST_METHOD(Pool_HasAtLeastOneObject)
	{
		TestPool->InitializePool(AActor::StaticClass(), PoolSize);
		ASSERT_THAT(IsTrue(TestPool->GetPoolSize() != 0));
	}
};
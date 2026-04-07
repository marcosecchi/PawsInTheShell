// Mastering Unreal Engine 5 Game Development with C++ Scripting
// Packt Publishing 2025
// Author: Marco Secchi (https://github.com/marcosecchi)

#include "CQTest.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Utils/PITS_FixedActorPool.h"

TEST_CLASS(PITS_FixedActorPool, "PawsInTheShell.Utils")
{
    UPITS_FixedActorPool* TestPool = nullptr;
	int32 PoolSize = 5;
	
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

	TEST_METHOD(Pool_Exists)
	{
		ASSERT_THAT(IsTrue(TestPool != nullptr));
	}

	TEST_METHOD(Pool_Initialized)
	{
		TestPool->InitializePool(AActor::StaticClass(), PoolSize);
		ASSERT_THAT(IsTrue(TestPool->GetPoolSize() == PoolSize));
	}
	
	TEST_METHOD(Pool_HasPooledObjects)
	{
		TestPool->InitializePool(AActor::StaticClass(), PoolSize);
		ASSERT_THAT(IsTrue(TestPool->HasAvailableObjectsInPool()));
	}

	TEST_METHOD(Pool_CanGetPooledObject)
	{
		TestPool->InitializePool(AActor::StaticClass(), PoolSize);
		const AActor* Actor = TestPool->GetObjectFromPool();
		ASSERT_THAT(IsTrue(Actor != nullptr));
	}

	TEST_METHOD(Pool_HasNoPooledObjects)
	{
		TestPool->InitializePool(AActor::StaticClass(), PoolSize);
		for (int32 i = 0; i < PoolSize; i++)
		{
			TestPool->GetObjectFromPool();
		}
		ASSERT_THAT(IsFalse(TestPool->HasAvailableObjectsInPool()));
	}

	TEST_METHOD(Pool_HasReleasedObjectFromPool)
	{
		TestPool->InitializePool(AActor::StaticClass(), PoolSize);
		for (int32 i = 0; i < PoolSize - 1; i++)
		{
			TestPool->GetObjectFromPool();
		}
		AActor* Actor = TestPool->GetObjectFromPool();
		TestPool->ReleaseObjectToPool(Actor);
		ASSERT_THAT(IsTrue(TestPool->HasAvailableObjectsInPool()));
	}
};
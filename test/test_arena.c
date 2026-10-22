#include "base_types.h"
#include "unity/unity.h"
#include "unity/unity_internals.h"

#include "arena.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_init(void)
{
    Arena arena;
    TEST_ASSERT_EQUAL(TRUE, arenaCreate(0, &arena));
    TEST_ASSERT_EQUAL(0, arena.baseCapacity);
    TEST_ASSERT_NOT_NULL(arena.pRootChunk);
    Arena arena2;
    TEST_ASSERT_EQUAL(TRUE, arenaCreate(1, &arena2));
    TEST_ASSERT_NOT_EQUAL(NULL, arenaAllocate(&arena2, 2));
    TEST_ASSERT_NOT_NULL(arena2.pRootChunk->pNext);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init);

    return (UNITY_END() != 0) ? 1 : 0;
}

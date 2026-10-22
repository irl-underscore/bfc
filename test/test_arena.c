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

void test_alloc(void)
{
    Arena arena;
    arenaCreate(100, &arena);
    TEST_ASSERT_EQUAL(FALSE, arenaAllocate(&arena, 0));
    void *pTemp = arenaAllocate(&arena, 400);
    TEST_ASSERT_NOT_NULL(pTemp);
    TEST_ASSERT_EQUAL(TRUE, arenaRealloc(&arena, &pTemp, 400, 800));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_alloc);

    return (UNITY_END() != 0) ? 1 : 0;
}

#include "base_types.h"
#include "unity/unity.h"
#include "unity/unity_internals.h"

#include "arena.h"
#include "dyn_string.h"

static Arena stringArena;

void setUp(void)
{
    arenaCreate(1024, &stringArena);
}

void tearDown(void)
{
    arenaDestroy(&stringArena);
}

void test_init(void)
{
    DynString str;
    TEST_ASSERT_EQUAL(FALSE, dynStringCreate(0, &stringArena, &str));
    TEST_ASSERT_EQUAL(TRUE, dynStringCreate(1, &stringArena, &str));
    TEST_ASSERT_EQUAL(TRUE, dynStringAppend(&str, "Hello"));
    TEST_ASSERT_EQUAL(TRUE, dynStringAppendFmt(&str, " W%s", "orld"));
    TEST_ASSERT_EQUAL_STRING("Hello World", str.pData);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init);

    return (UNITY_END() != 0) ? 1 : 0;
}

// enjector.cpp : Defines the entry point for the application.
//

#include <enjector/core/test.h>

void should_successfully_test_equality_bool() {
    bool actual = true;
    TEST_ASSERT_EQUAL_BOOL(true, actual);
    TEST_ASSERT_EQUAL_BOOL_FATAL(true, actual);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_TRUE_FATAL(actual);
}

int main()
{

    should_successfully_test_equality_bool();
    return 0;
}

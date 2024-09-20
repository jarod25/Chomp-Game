#ifndef TESTS_MACRO_H
#define TESTS_MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        printf("[AssertEqual] Test failed: expected %d, got %d\n", (expected), (actual)); \
    } else { \
        printf("[AssertEqual] Test passed: %d == %d\n", (expected), (actual)); \
    }

#define ASSERT_NOT_EQ(expected, actual) \
    if ((expected) == (actual)) { \
        printf("[AssertNotEqual] Test failed: expected %d, got %d\n", (expected), (actual)); \
    } else { \
        printf("[AssertNotEqual] Test passed: %d != %d\n", (expected), (actual)); \
    }

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        printf("[AssertTrue] Test failed: condition is false\n"); \
    } else { \
        printf("[AssertTrue] Test passed: condition is true\n"); \
    }

#define ASSERT_FALSE(condition) \
    if (!(condition)) { \
        printf("[AssertFalse] Test passed: condition is false\n"); \
    } else { \
        printf("[AssertFalse] Test failed: condition is true\n"); \
    }

#endif // TESTS_MACRO_H

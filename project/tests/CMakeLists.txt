FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")

FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(include)

ADD_EXECUTABLE(
        tests
        src/main.cpp src/utils.cpp
        src/tests_base.cpp src/tests_base_neg.cpp
        src/tests_math_base.cpp src/tests_math_base_neg.cpp
        src/tests_math_extra.cpp src/tests_math_extra_neg.cpp
)
TARGET_LINK_LIBRARIES(tests ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} prepmatrix stdc++fs)

ADD_TEST(
        NAME BaseTest
        COMMAND tests ${CMAKE_SOURCE_DIR}/project/tests/data --gtest_filter=-*Extra*
)
ADD_TEST(
        NAME ExtraTest
        COMMAND tests ${CMAKE_SOURCE_DIR}/project/tests/data --gtest_filter=*Extra*
)

cmake_minimum_required(VERSION 3.0.2)
project(BeamMeUp)
set(VARIANT_DYNAMIC_THREAD_UNSAFE ${PROJECT_NAME})
set(VARIANT_STATIC_THREAD_UNSAFE ${PROJECT_NAME}Static)
set(VARIANT_DYNAMIC_THREAD_SAFE ${PROJECT_NAME}Ts)
set(VARIANT_STATIC_THREAD_SAFE ${PROJECT_NAME}StaticTs)

OPTION(GENERATE_COVERAGE "Generate coverage" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(NOT COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++1z" COMPILER_SUPPORTS_CXX1Z)
endif()

IF(COMPILER_SUPPORTS_CXX14)
    add_compile_options(-std=c++14)
ELSEIF(COMPILER_SUPPORTS_CXX1Z)
    add_compile_options(-std=c++1z)
ELSE()
    message(FATAL_ERROR "C++14 is required.")
ENDIF()

include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/
)

enable_testing()
find_package(GTest)
find_package(GMock)

set(LOGIC_SOURCE_FILES
    source/ArbitraryPointer.cpp
    include/beammeup/ArbitraryPointer.h
    source/Receiver.cpp
    include/beammeup/Receiver.h
    source/Signaler.cpp
    include/beammeup/Signaler.h
    source/Transporter.cpp
    include/beammeup/Transporter.h
    include/beammeup/Types.h
    source/Variant.cpp
    include/beammeup/Variant.h
    include/beammeup/VariantMap.h
    source/VariantVector.cpp
    include/beammeup/VariantVector.h
)
set(TEST_SOURCE_FILES
    tests/mocks/MockTransporter.h
    tests/stubs/StubSmartObject.cpp
    tests/stubs/StubSmartObject.h
    tests/stubs/StubTrackedPointer.cpp
    tests/stubs/StubTrackedPointer.h
    tests/TestArbitraryPointer.cpp
    tests/TestSignals.cpp
    tests/TestTransporter.cpp
    tests/TestVariant.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(LOGIC_SOURCE_FILES_TS
    ${LOGIC_SOURCE_FILES}
    source/Thread.cpp
    include/beammeup/Thread.h
)
set(TEST_SOURCE_FILES_TS
    ${TEST_SOURCE_FILES}
    tests/mocks/MockMutex.h
    tests/TestThread.cpp
)

## Thread unsafe version
# static
add_library(${VARIANT_STATIC_THREAD_UNSAFE} STATIC ${LOGIC_SOURCE_FILES})
# dynamic
add_library(${VARIANT_DYNAMIC_THREAD_UNSAFE} SHARED ${LOGIC_SOURCE_FILES})

## Thread safe version
# static
add_library(${VARIANT_STATIC_THREAD_SAFE} STATIC ${LOGIC_SOURCE_FILES_TS})
set_target_properties(${VARIANT_STATIC_THREAD_SAFE} PROPERTIES COMPILE_DEFINITIONS "THREAD_SAFE=1")
# dynamic
add_library(${VARIANT_DYNAMIC_THREAD_SAFE} SHARED ${LOGIC_SOURCE_FILES_TS})
set_target_properties(${VARIANT_DYNAMIC_THREAD_SAFE} PROPERTIES COMPILE_DEFINITIONS "THREAD_SAFE=1")

## Tests
IF(GTEST_FOUND AND GMOCK_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
    include_directories(${GMOCK_INCLUDE_DIRS})
    add_custom_target(tests
            COMMENT "Running tests."
            COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${VARIANT_STATIC_THREAD_SAFE}_tests
            COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${VARIANT_STATIC_THREAD_UNSAFE}_tests
            DEPENDS ${VARIANT_STATIC_THREAD_SAFE}_tests
            DEPENDS ${VARIANT_STATIC_THREAD_UNSAFE}_tests
            )

    IF(CMAKE_SYSTEM_NAME STREQUAL Darwin)
        add_custom_target(coverage
                COMMENT "Generating code coverage report."
                COMMAND cmake -DGENERATE_COVERAGE=on .
                COMMAND make clean
                COMMAND find ./CMakeFiles/ -name *.gcda -delete
                COMMAND rm -rf ./code_coverage.info ./code_coverage_report/ *.profraw
                COMMAND make tests
                COMMAND lcov -d ./CMakeFiles/ -d ./source/ --no-external --capture --output-file ./code_coverage.info -rc lcov_branch_coverage=1
                COMMAND genhtml code_coverage.info --branch-coverage --output-directory ./code_coverage_report/
                COMMAND open ./code_coverage_report/index.html
                COMMAND cmake .
                COMMAND make clean
                )
    ENDIF()

    IF(GENERATE_COVERAGE)
        MESSAGE(STATUS "Generating code coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -fprofile-instr-generate -fcoverage-mapping")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -fprofile-instr-generate -fcoverage-mapping")
    ENDIF()

    # thread safe
    add_executable(${VARIANT_STATIC_THREAD_UNSAFE}_tests ${TEST_SOURCE_FILES} ${LOGIC_SOURCE_FILES})
    target_link_libraries(${VARIANT_STATIC_THREAD_UNSAFE}_tests ${GMOCK_LIBRARIES} ${GTEST_BOTH_LIBRARIES} Threads::Threads)
    set_target_properties(${VARIANT_STATIC_THREAD_UNSAFE}_tests PROPERTIES EXCLUDE_FROM_ALL 1)
    target_compile_definitions(${VARIANT_STATIC_THREAD_UNSAFE}_tests PRIVATE VIRTUAL_FOR_MOCKING=1)
    # thread unsafe
    add_executable(${VARIANT_STATIC_THREAD_SAFE}_tests ${TEST_SOURCE_FILES_TS} ${LOGIC_SOURCE_FILES_TS})
    set_target_properties(${VARIANT_STATIC_THREAD_SAFE}_tests PROPERTIES EXCLUDE_FROM_ALL 1)
    target_compile_definitions(${VARIANT_STATIC_THREAD_SAFE}_tests PRIVATE THREAD_SAFE=1 VIRTUAL_FOR_MOCKING=1)
    target_link_libraries(${VARIANT_STATIC_THREAD_SAFE}_tests ${GMOCK_LIBRARIES} ${GTEST_BOTH_LIBRARIES} Threads::Threads)
ELSE()
    message(STATUS "Missing Google Test / Google Mock. Tests disabled.")
ENDIF()

IF(NOT CMAKE_SYSTEM_NAME STREQUAL Windows)
    set_target_properties(${VARIANT_STATIC_THREAD_UNSAFE} PROPERTIES OUTPUT_NAME ${VARIANT_DYNAMIC_THREAD_UNSAFE})
    set_target_properties(${VARIANT_STATIC_THREAD_SAFE} PROPERTIES OUTPUT_NAME ${VARIANT_DYNAMIC_THREAD_SAFE})
ENDIF()

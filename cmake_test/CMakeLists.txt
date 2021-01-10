cmake_minimum_required (VERSION 3.19.1)

# sets the PROJECT_NAME variable:
project (cmake_test)

# in the *.input file we have, we have a sort of 'if' check that will enable the
# define based on whether option is ON or OFF
option (__VALUE__ "Pass parameters by value to add function" OFF)
option (__REFERENCE__ "Pass parameters by reference to add function" OFF)
option (__POINTER__ "Pass parameters as pointers to add function" OFF)

configure_file ("${PROJECT_SOURCE_DIR}/add_by.h.input" "${PROJECT_SOURCE_DIR}/add_by.h")

# this basically adds a -I option to the compile command 
include_directories ("${PROJECT_SOURCE_DIR}")

if (__VALUE__)
    include_directories ("${PROJECT_SOURCE_DIR}/add_by_value")
    add_subdirectory ("add_by_value")
    set (EXTRA_LIBS ${EXTRA_LIBS} add_by_value)
endif (__VALUE__)

if (__REFERENCE__)
    include_directories ("${PROJECT_SOURCE_DIR}/add_by_reference")
    add_subdirectory ("add_by_reference")
    set (EXTRA_LIBS ${EXTRA_LIBS} add_by_reference)
endif (__REFERENCE__)

if (__POINTER__)
    include_directories ("${PROJECT_SOURCE_DIR}/add_by_pointer")
    add_subdirectory ("add_by_pointer")
    set (EXTRA_LIBS ${EXTRA_LIBS} add_by_pointer)
endif (__POINTER__)

add_executable (cmake_test cmake_test.cpp)

target_link_libraries (cmake_test ${EXTRA_LIBS})
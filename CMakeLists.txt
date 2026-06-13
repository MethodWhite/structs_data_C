cmake_minimum_required(VERSION 3.14)
project(ds_data_structures VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

enable_testing()

option(DS_BUILD_TESTS "Build tests" ON)

add_library(ds_data_structures
    src/ds_array_list.c
    src/ds_hash_table.c
    src/ds_tensor.c
    src/ds_matrix.c
    src/ds_heap.c
    src/ds_ring_buffer.c
)

target_include_directories(ds_data_structures PUBLIC include)

if(DS_BUILD_TESTS)
    add_executable(ds_test_array_list
        examples/test_array_list.c
    )
    target_link_libraries(ds_test_array_list PRIVATE ds_data_structures)
    add_test(NAME ds_test_array_list COMMAND ds_test_array_list)

    add_executable(ds_test_hash_table
        examples/test_hash_table.c
    )
    target_link_libraries(ds_test_hash_table PRIVATE ds_data_structures)
    add_test(NAME ds_test_hash_table COMMAND ds_test_hash_table)

    add_executable(ds_test_tensor
        examples/test_tensor.c
    )
    target_link_libraries(ds_test_tensor PRIVATE ds_data_structures)
    add_test(NAME ds_test_tensor COMMAND ds_test_tensor)

    add_executable(ds_test_matrix
        examples/test_matrix.c
    )
    target_link_libraries(ds_test_matrix PRIVATE ds_data_structures)
    add_test(NAME ds_test_matrix COMMAND ds_test_matrix)

    add_executable(ds_test_heap
        examples/test_heap.c
    )
    target_link_libraries(ds_test_heap PRIVATE ds_data_structures)
    add_test(NAME ds_test_heap COMMAND ds_test_heap)

    add_executable(ds_test_ring_buffer
        examples/test_ring_buffer.c
    )
    target_link_libraries(ds_test_ring_buffer PRIVATE ds_data_structures)
    add_test(NAME ds_test_ring_buffer COMMAND ds_test_ring_buffer)
endif()

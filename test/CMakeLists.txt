find_package(Catch2 REQUIRED) # catch2 as system package requirement
set(DATA_STRUCT_TEST_FILES structures/test-vector3D.cpp)

# add executable app from source files
add_executable(data_structures_test test-main.cpp ${DATA_STRUCT_TEST_FILES})

#link target (executable) with catch2 by private flag
target_link_libraries(data_structures_test PRIVATE Catch2::Catch2WithMain data_structures)

#create test named 'struct_test' invokable through ctest
add_test(NAME struct_test COMMAND data_structures_test)

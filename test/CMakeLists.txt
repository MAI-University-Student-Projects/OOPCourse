find_package(Catch2 REQUIRED) # catch2 as system package requirement
set(STRUCT_TEST_FILES structures/test-vector3D.cpp)

# add executable app from source files
add_executable(oop_test test-main.cpp ${STRUCT_TEST_FILES} test-figures.cpp)

#link target (executable) with catch2 by private flag
target_link_libraries(oop_test PRIVATE Catch2::Catch2WithMain data_structures figures)

#create test named 'oop_test' invokable through ctest
add_test(NAME oop_ctest COMMAND oop_test)

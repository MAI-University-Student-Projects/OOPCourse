set(DATA_STRUCT_FILES structures/vector3D.h structures/vector3D.cpp)

add_library(data_structures ${DATA_STRUCT_FILES})

add_executable(oop_app main.cpp)

target_link_libraries(oop_app data_structures)
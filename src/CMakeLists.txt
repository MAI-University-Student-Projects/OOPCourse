set(STRUCT_HEADERS structures/vector3D.h)
set(STRUCT_SOURCES structures/vector3D.cpp)

set(FIGURE_HEADERS figures/figure.h figures/pentagon.h figures/rhombus.h figures/trapezoid.h)
set(FIGURE_SOURCES figures/pentagon.cpp figures/rhombus.cpp figures/trapezoid.cpp)

add_library(data_structures ${STRUCT_SOURCES} ${STRUCT_HEADERS})
add_library(figures ${FIGURE_HEADERS} ${FIGURE_SOURCES})

add_executable(oop_app main.cpp)

target_link_libraries(oop_app data_structures figures)

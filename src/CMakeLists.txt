include_directories(${PROJECT_SOURCE_DIR}/inc)

file(GLOB ALL_SRC
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )


add_executable(systa ${ALL_SRC})
target_link_libraries(systa mosquitto)
# Set the project name
project (ctsp_io)


file(GLOB SOURCES 
    "src/TSPLIB_instance.cpp"
    "src/io_files.cpp"
    "src/CTSP_instance.cpp"
    "src/sync_heuristic_a_input.cpp"
    "src/CTSP_2_heuristic_a_input.cpp"
)


# Add a library with the above sources
add_library(${PROJECT_NAME} 
    ${SOURCES})


    
add_library(sub::ctsp_io ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    sub::gomautil
)

# Add flags to the compiler
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall 
  -Wextra
  -Wshadow
)

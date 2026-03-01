# Set the project name
project (heuristic)

file(GLOB SOURCES 
    "src/constructive_TSP_based.cpp"
    "src/reduce_fitness.cpp"
    "src/sync_heuristic.cpp"
)

# Add a library with the above sources
add_library(${PROJECT_NAME} 
    ${SOURCES})
    
add_library(sub::heuristic ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    sub::gomautil
    sub::ctsp_io
    sub::ctsp_sol
)

# Add flags to the compiler
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall 
  -Wextra
  -Wshadow
)

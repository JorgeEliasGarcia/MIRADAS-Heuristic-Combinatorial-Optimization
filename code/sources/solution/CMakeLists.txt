# Set the project name
project (ctsp_sol)


file(GLOB SOURCES 
    "src/sync_plan.cpp"
    "src/sync_solution.cpp"
)


# Add a library with the above sources
add_library(${PROJECT_NAME} 
    ${SOURCES})


    
add_library(sub::ctsp_sol ALIAS ${PROJECT_NAME})


target_link_libraries(${PROJECT_NAME}
    sub::ctsp_io
)
 
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Add flags to the compiler
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall 
  -Wextra
  -Wshadow
)

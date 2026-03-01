project(ctsp1_heuristic)

add_executable(${PROJECT_NAME}
src/main.cpp 
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
 
target_link_libraries(
    ${PROJECT_NAME}
    sub::gomautil
    sub::ctsp_io
    sub::heuristic
)  

# Add flags to the compiler
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall 
  -Wextra
  -Wshadow
)

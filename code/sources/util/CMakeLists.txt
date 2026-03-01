# Set the project name
project (gomautil)

file(GLOB SOURCES 
    "src/matrix.cpp"
)

# Add a library with the above sources

add_library(${PROJECT_NAME} 
    ${SOURCES})

add_library(sub::gomautil ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Add flags to the compiler
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall 
  -Wextra
  -Wshadow 
)

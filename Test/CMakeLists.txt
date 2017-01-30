include_directories("../Framework")

file(GLOB_RECURSE SRC_LIST "*.cpp")
file(GLOB_RECURSE HEADER_LIST "*.h")
add_executable(Tests ${SRC_LIST} ${HEADER_LIST})
add_test(NAME ${PROJECT_NAME}Tests COMMAND Tests)
target_link_libraries (Tests ${PROJECT_NAME})

# The test is run in the build directory. So the test data is copied there too
add_custom_command(TARGET Tests POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/InvalidTest.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ValidTest.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)


enable_testing()

add_custom_target(tests)

add_executable(pipe_test EXCLUDE_FROM_ALL Tests/Pipe.cxx)
add_dependencies(tests pipe_test)
target_link_libraries(pipe_test posix)
add_test(NAME pipe COMMAND pipe_test)

add_executable(poll_test EXCLUDE_FROM_ALL Tests/Poll.cxx)
add_dependencies(tests poll_test)
target_link_libraries(poll_test posix)
add_test(NAME poll COMMAND poll_test)

include_directories (${SRC_DIR}/include)
link_directories (${SRC_DIR}/test ${SRC_DIR}/src)
add_executable (${PROJECT_NAME}-tests
  tests.cpp
  components.cpp
  position_iterator_test.cpp
  reading_iterator_test.cpp
  rotor_test.cpp
)
target_link_libraries (${PROJECT_NAME}-tests gmock pthread)


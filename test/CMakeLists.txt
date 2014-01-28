include_directories (${SRC_DIR}/include)
link_directories (${SRC_DIR}/test ${SRC_DIR}/src)
add_executable (${PROJECT_NAME}-tests
  tests.cpp
  components.cpp
  position_iterator_test.cpp
  reading_iterator_test.cpp
  writing_iterator_test.cpp
  rotor_lock_test.cpp
  rotor_mutex_test.cpp
  rotor_state_test.cpp
  rotor_test.cpp
  type_list_test.cpp
  builder_test.cpp
  scene_test.cpp
)
target_link_libraries (${PROJECT_NAME}-tests gmock pthread)


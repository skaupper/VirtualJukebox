find_program(
  CLANG_FORMAT_EXE
  NAMES "clang-format"
  DOC "Path to clang-format executable"
  )
if(NOT CLANG_FORMAT_EXE)
  message(FATAL_ERROR "clang-format not found.")
else()
  message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")
endif()

if(CLANG_FORMAT_EXE)
  add_custom_target(
          clang-format
          COMMAND clang-format
          -style=Google
          -i ${ALL_SOURCE_FILES}
  )
endif()

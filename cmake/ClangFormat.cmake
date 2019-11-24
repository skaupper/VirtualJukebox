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

# Generate absolute paths
set(CLANG_FORMAT_SOURCES_RELATIVE ${SOURCES} ${TEST_SOURCES})
foreach(source ${CLANG_FORMAT_SOURCES_RELATIVE})
  get_filename_component(source ${source} ABSOLUTE)
  list(APPEND CLANG_FORMAT_SOURCES_ABS ${source})
endforeach()

add_custom_target(
  clang-format
  COMMAND clang-format
  -style=Google
  -i
  ${CLANG_FORMAT_SOURCES_ABS}
)

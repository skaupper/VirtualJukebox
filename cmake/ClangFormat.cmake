find_program(
  CLANG_FORMAT_EXE
  NAMES "clang-format-6.0"
  DOC "Path to clang-format-6.0 executable"
  )
if(NOT CLANG_FORMAT_EXE)
  message(FATAL_ERROR "clang-format-6.0 not found.")
endif()

function (clang_format_add_target)
  # Generate absolute paths
  set(FORMATTING_SOURCES ${ARGN}) # ARGN contains all parameters past the last one
  set(CLANG_FORMAT_SOURCES_RELATIVE ${FORMATTING_SOURCES})
  foreach(source ${CLANG_FORMAT_SOURCES_RELATIVE})
    get_filename_component(source ${source} ABSOLUTE)
    list(APPEND CLANG_FORMAT_SOURCES_ABS ${source})
  endforeach()

  add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT_EXE}
    -style=file
    -i
    ${CLANG_FORMAT_SOURCES_ABS}
  )
endfunction()

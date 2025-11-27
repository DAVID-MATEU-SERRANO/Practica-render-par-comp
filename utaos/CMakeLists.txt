set(COMMON_SRC_FILES 
  "${CMAKE_SOURCE_DIR}/aos/src/main.cpp"
)

set(CURRENT_DIR_SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/test_pixel_aos.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_aos_logic.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_aos_ppm_smoke.cpp"
)

add_unit_test_target(
  TARGET_NAME   utaos
  SOURCE_FILES  ${COMMON_SRC_FILES} ${CURRENT_DIR_SRC_FILES}
  LIBRARY_FILTER aos
  COVERAGE_DIR  coverage-aos
  LIBRARY_TO_LINK common
  INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/common/include
    ${CMAKE_SOURCE_DIR}/aos/include
)
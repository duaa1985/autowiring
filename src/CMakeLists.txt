include_directories(
  # Need to make sure all of our tests can find gtest
  ${PROJECT_SOURCE_DIR}/contrib/gtest-1.7.0/fused-src

  # All projects in this folder have named access to other projects here
  .
)

add_subdirectory(autoboost)
add_subdirectory(autonet)
add_subdirectory(autotesting)
add_subdirectory(autowiring)
add_subdirectory(benchmark)

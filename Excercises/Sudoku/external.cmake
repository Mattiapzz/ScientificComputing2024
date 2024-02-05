include(ExternalProject)

ExternalProject_Add(fmt
  PREFIX ${THIRD_PARTY_DIR}
  SOURCE_DIR ${THIRD_PARTY_DIR}/fmt
  INSTALL_DIR ${THIRD_PARTY_DIR_USR}
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG e69e5f9
  GIT_SHALLOW TRUE
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DFMT_DOC=OFF
    -DFMT_TEST=OFF
    -DFMT_INSTALL=ON
    -DCMAKE_INSTALL_PREFIX=../../usr
)

# add_dependencies(fmt)
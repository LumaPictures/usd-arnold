# Simplistic FindKatana cmake module, based on the USD one.

find_path(KATANA_API_BASE_DIR
          NAMES katana
          HINTS "${KATANA_HOME}" "$ENV{KATANA_HOME}" "${KATANA_API_LOCATION}" "$ENV{KATANA_API_LOCATION}"
          DOC "Katana API base directory.")

find_path(KATANA_API_INCLUDE_DIR
          NAMES FnAPI/FnAPI.h
          HINTS "${KATANA_API_LOCATION}" "$ENV{KATANA_API_LOCATION}" "${KATANA_API_BASE_DIR}"
          PATH_SUFFIXES plugin_apis/include/
          DOC "Katana API include directory.")

find_path(KATANA_API_SOURCE_DIR
          NAMES FnConfig/FnConfig.cpp
          HINTS "${KATANA_API_LOCATION}" "$ENV{KATANA_API_LOCATION}" "${KATANA_API_BASE_DIR}"
          PATH_SUFFIXES plugin_apis/src
          DOC "Katana API source directory.")

if(KATANA_API_INCLUDE_DIR AND EXISTS "${KATANA_API_INCLUDE_DIR}/FnAPI/FnAPI.h")
    foreach(_comp MAJOR MINOR RELEASE)
        file(STRINGS
             ${KATANA_API_INCLUDE_DIR}/FnAPI/FnAPI.h
             _TMP
             REGEX "#define KATANA_VERSION_${_comp} .*$")
        string(REGEX MATCHALL "[0-9]+" KATANA_API_${_comp}_VERSION ${_TMP})
    endforeach()
    set(KATANA_API_VERSION ${KATANA_API_MAJOR_VERSION}.${KATANA_API_MINOR_VERSION}.${KATANA_API_RELEASE_VERSION})
endif()

message(STATUS "Katana API: ${KATANA_API_BASE_DIR}")
message(STATUS "Katana API includes: ${KATANA_API_INCLUDE_DIR}")
message(STATUS "Katana API source: ${KATANA_API_SOURCE_DIR}")
message(STATUS "Katana API version: ${KATANA_API_VERSION}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        Katana
        REQUIRED_VARS KATANA_API_BASE_DIR KATANA_API_INCLUDE_DIR KATANA_API_SOURCE_DIR
        VERSION_VAR KATANA_API_VERSION)

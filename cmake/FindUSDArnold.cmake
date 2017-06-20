# Simple module to find USD.


find_path(USD_ARNOLD_INCLUDE_DIR pxr/usd/usdAi/api.h
          DOC "USD Arnold Include directory")

find_library(USD_ARNOLD_LIBRARY libusdAi.so
             DOC "USD Arnold Librarires directory")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
        USDArnold
        REQUIRED_VARS
        USD_ARNOLD_INCLUDE_DIR
        USD_ARNOLD_LIBRARY)

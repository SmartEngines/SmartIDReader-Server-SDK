cmake_minimum_required(VERSION 2.8)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

project(smartid_php_wrapper)

find_library(SMARTID_LIBRARY mockSmartIdEngine HINTS ${SMARTID_LIBRARY_PATH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/smartIdEngine)

execute_process(COMMAND ${PHP_CONFIG_PATH} --includes
  OUTPUT_VARIABLE PHP_INCLUDE_DIRS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(COMMAND ${PHP_CONFIG_PATH} --php-binary
  OUTPUT_VARIABLE PHP_EXECUTABLE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "variable is:  ${PHP_CONFIG_PATH}")

execute_process(COMMAND ${PHP_CONFIG_PATH} --version
  OUTPUT_VARIABLE PHP_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "variable is:  ${PHP_VERSION}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++11 ${PHP_INCLUDE_DIRS}")

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

set(MY_SWIG_FILE phpSmartIdEngine.i)
set_property(SOURCE ${MY_SWIG_FILE} PROPERTY CPLUSPLUS ON)

if(${PHP_VERSION} STRGREATER "7")
  swig_add_module(phpSmartIdEngine php7 ${MY_SWIG_FILE})

else()
swig_add_module(phpSmartIdEngine php ${MY_SWIG_FILE})

endif()
SET_TARGET_PROPERTIES(${SWIG_MODULE_phpSmartIdEngine_REAL_NAME} PROPERTIES PREFIX "")

target_link_libraries(phpSmartIdEngine ${SMARTID_LIBRARY})
swig_link_libraries(phpSmartIdEngine ${SMARTID_LIBRARY})

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/bin")

install(TARGETS phpSmartIdEngine
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
)

install(
  CODE "FILE(GLOB php_files \"${CMAKE_CURRENT_BINARY_DIR}/*.php\")"
  CODE "FILE(INSTALL \${php_files} DESTINATION \${CMAKE_INSTALL_PREFIX}/bindings/php)"
)

enable_testing()

add_test(NAME smartid_sample_php
COMMAND ${PHP_EXECUTABLE} -c php.ini ${CMAKE_SOURCE_DIR}/smartid_sample.php
"${CMAKE_SOURCE_DIR}/../../testdata/passport_rf_2.jpg"
"${CMAKE_SOURCE_DIR}/../../data-zip/bundle_mock_smart_idreader.zip"
"rus.passport.national"
WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
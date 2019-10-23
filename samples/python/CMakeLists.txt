cmake_minimum_required(VERSION 2.8)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

project(smartid_python_wrapper)

find_library(SMARTID_LIBRARY mockSmartIdEngine HINTS ${SMARTID_LIBRARY_PATH})
message("${SMARTID_LIBRARY}")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/smartIdEngine)

find_package(PythonInterp)
find_package(PythonLibs)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(../smartIdEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++11")

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

set(MY_SWIG_FILE smartIdEngine.i)
set_property(SOURCE ${MY_SWIG_FILE} PROPERTY CPLUSPLUS ON)

SWIG_ADD_MODULE(pySmartIdEngine python  ${MY_SWIG_FILE})

set_target_properties(${SWIG_MODULE_pySmartIdEngine_REAL_NAME} PROPERTIES PREFIX "")

set_property(TARGET _pySmartIdEngine PROPERTY GENERATED_COMPILE_OPTIONS "-fPIC")

target_link_libraries(_pySmartIdEngine ${SMARTID_LIBRARY})

SWIG_LINK_LIBRARIES(pySmartIdEngine ${SMARTID_LIBRARY})
target_link_libraries(_pySmartIdEngine ${PYTHON_LIBRARIES})

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/../../")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/bin")

install(TARGETS _pySmartIdEngine
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
)

install(
  CODE "FILE(GLOB python_files \"${CMAKE_CURRENT_BINARY_DIR}/*.py\")"
  CODE "FILE(INSTALL \${python_files} DESTINATION \${CMAKE_INSTALL_PREFIX}/bindings/python)"
)

enable_testing()

add_test(NAME smartid_sample_python
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/smartid_sample.py
  "${CMAKE_SOURCE_DIR}/../../testdata/passport_rf_2.jpg"
  "${CMAKE_SOURCE_DIR}/../../data-zip/bundle_mock_smart_idreader.zip"
  "rus.passport.national"
)
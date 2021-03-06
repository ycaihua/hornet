cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("hornet")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

find_package(PkgConfig)

include_directories("include")

# Java
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

# LLVM
find_package(LLVM 3.5)

if(LLVM_FOUND)
  include_directories(${LLVM_INCLUDE_DIRS})
  set(SRC ${SRC} java/llvm.cc)
  set(LIBS ${LIBS} ${LLVM_LIBRARIES})
  set(LIBS ${LIBS} ${LLVM_SYSTEM_LIBS})
  add_definitions(-DCONFIG_HAVE_LLVM)
endif()

# FFI
pkg_check_modules(LIBFFI REQUIRED libffi)
include_directories(${LIBFFI_INCLUDE_DIRS})

set(CMAKE_CFLAGS "-O3 -std=c11 -g")
set(CMAKE_CXX_FLAGS "-O3 -std=c++11 -g")

add_library(jvm STATIC
  include/hornet/byte-order.hh
  include/hornet/compat.hh
  include/hornet/java.hh
  include/hornet/jni.hh
  include/hornet/opcode.hh
  include/hornet/os.hh
  include/hornet/system_error.hh
  include/hornet/translator.hh
  include/hornet/vm.hh
  include/hornet/zip.hh

  vm/alloc.cc
  vm/jvm.cc
  vm/klass.cc
  vm/object.cc
  vm/thread.cc

  mps/mps.c

  java/backend.cc
  java/class_file.cc
  java/constant_pool.cc
  java/ffi.cc
  java/interp.cc
  java/jni.cc
  java/loader.cc
  java/opcode.cc
  java/prims.cc
  java/translator.cc
  java/verify.cc
  java/zip.cc

  ${SRC}
  )

add_executable(hornet hornet.cc)

target_link_libraries(hornet jvm z pthread ${LIBS})
target_link_libraries(hornet ${LIBFFI_LIBRARIES})

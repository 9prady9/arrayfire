# Copyright (c) 2021, ArrayFire
# All rights reserved.
#
# This file is distributed under 3-clause BSD license.
# The complete license agreement can be obtained at:
# http://arrayfire.com/licenses/BSD-3-Clause
#
# This Find Module expects/assumes an AOCL installation available
# via an environment variable AOCL_ROOT. If one is not provided, it
# will try to locate the headers and libraries in standard paths. If
# no path has the required files, no targets are created and *_FOUND
# variables are set to OFF/False
#
# Targets defined by this Find Module grouped w.r.t component of this find module
#
# linalg (Linear Algebra)
#   AOCL::blis
#   AOCL::blis_static
#   AOCL::blis_mt
#   AOCL::blis_mt_static
#   AOCL::flame
#   AOCL::flame_static
#   AOCL::lapacke_static
#   AOCL::scalapack        # For distributed memory machines like AMD Epyc family CPUs
#   AOCL::scalapack_static # For distributed memory machines like AMD Epyc family CPUs
#   AOCL::sparse
#   AOCL::sparse_static
#
# fft (Fast Fourier Transforms)
#   AOCL::fftw          # double precision lib
#   AOCL::fftw_static   # double precision lib
#   AOCL::fftwf         # single precision lib
#   AOCL::fftwf_static  # single precision lib
#   AOCL::fftwl         # long double(80 bit extended) precision lib
#   AOCL::fftwl_static  # long double(80 bit extended) precision lib
#   AOCL::fftwq         # Quadruple(128 bit) precision lib
#   AOCL::fftwq_static  # Quadruple(128 bit) precision lib
#   AOCL::fftw_mt          # double precision lib with openmp threads
#   AOCL::fftw_mt_static   # double precision lib with openmp threads
#   AOCL::fftwf_mt         # single precision lib with openmp threads
#   AOCL::fftwf_mt_static  # single precision lib with openmp threads
#   AOCL::fftwl_mt         # long double(80 bit extended) precision lib with openmp threads
#   AOCL::fftwl_mt_static  # long double(80 bit extended) precision lib with openmp threads
#   AOCL::fftwq_mt         # Quadruple(128 bit) precision lib with openmp threads
#   AOCL::fftwq_mt_static  # Quadruple(128 bit) precision lib with openmp threads
#   AOCL::fftw_cl          # 64-bit lib with mpi support for cluster
#   AOCL::fftw_cl_static   # 64-bit lib with mpi support for cluster
#   AOCL::fftwf_cl         # 32-bit lib with mpi support for cluster
#   AOCL::fftwf_cl_static  # 32-bit lib with mpi support for cluster
#   AOCL::fftwl_cl         # 80 bit extended lib with mpi support for cluster
#   AOCL::fftwl_cl_static  # 80 bit extended lib with mpi support for cluster
#
# math (AMD Math library)
#   AOCL::math
#   AOCL::math_static
#
# rng (Random number generator libraries)
#   AOCL::rng
#   AOCL::rng_static
#   AOCL::rng_mt        # threaded support using openmp
#   AOCL::rng_mt_static # threaded support using openmp
#   AOCL::secure_rng
#   AOCL::secure_rng_static
#
# Sets the following variables:
#          AOCL_INCLUDE_DIR
#          AOCL_<UPPER_CASE_TARGET_NAME>_LINK_LIBRARY
#          AOCL_<UPPER_CASE_TARGET_NAME>_DLL - Windows only
#
# Usage:
#
# find_package(AOCL REQUIRED COMPONENTS linalg)
# if(AOCL_FOUND)
#   target_link_libraries(mylib PRIVATE AOCL::blis_mt AOCL::flame)
# endif()
#
# OR if you want to link against the static library:
#
# find_package(AOCL REQUIRED COMPONENTS rng)
# if(AOCL_FOUND)
#   target_link_libraries(mylib PRIVATE AOCL::rng_mt_static)
# endif()
#

find_path(AOCL_INCLUDE_DIR
  NAMES
    blis.h
  PATHS
    $ENV{AOCL_ROOT}/lib
    $ENV{HOME}/amd/aocl/3.0
    $ENV{HOME}/amd/aocl/3.0-6
  PATH_SUFFIXES
    include
  )
if(NOT AOCL_INCLUDE_DIR)
  message(FATAL_ERROR
      "Unable to find AOCL(blis) headers. Make sure AOCL 3.0 is installed")
endif()
mark_as_advanced(AOCL_INCLUDE_DIR)

set(_aocl_required_vars "")

function(find_aocl_library)
  set(options "STATIC_LIB")
  set(single_args NAME LIBRARY_NAME)
  set(multi_args "")

  cmake_parse_arguments(aocl_args "${options}" "${single_args}" "${multi_args}" ${ARGN})

  if(TARGET AOCL::${aocl_args_NAME})
    return()
  endif()
  string(TOUPPER ${aocl_args_NAME} TRGT_NAME)

  if(aocl_args_STATIC_LIB)
    set(LIB_SEARCH_NAME
        ${CMAKE_STATIC_LIBRARY_PREFIX}${aocl_args_LIBRARY_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
  else()
    set(LIB_SEARCH_NAME ${aocl_args_LIBRARY_NAME})
  endif()

  find_library(AOCL_${TRGT_NAME}_LINK_LIBRARY
    NAMES
      ${LIB_SEARCH_NAME}
    PATHS
      $ENV{AOCL_ROOT}/lib
      $ENV{HOME}/amd/aocl/3.0
      $ENV{HOME}/amd/aocl/3.0-6
    PATH_SUFFIXES
      lib
    )
  mark_as_advanced(AOCL_${TRGT_NAME}_LINK_LIBRARY)

  if(AOCL_${TRGT_NAME}_LINK_LIBRARY)
    # Append current link lib variable to parent required vars list
    set(required_vars ${_aocl_required_vars})
    list(APPEND required_vars AOCL_${TRGT_NAME}_LINK_LIBRARY)
    set(_aocl_required_vars ${required_vars} PARENT_SCOPE)

    if(aocl_args_STATIC_LIB)
      add_library(AOCL::${aocl_args_NAME} STATIC IMPORTED)
    else()
      add_library(AOCL::${aocl_args_NAME} SHARED IMPORTED)
    endif()
    set_target_properties(AOCL::${aocl_args_NAME}
      PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${AOCL_INCLUDE_DIR}"
        IMPORTED_LOCATION "${AOCL_${TRGT_NAME}_LINK_LIBRARY}"
        $<aocl_args_STATIC_LIB:IMPORTED_NO_SONAME TRUE>
      )
  else()
    message(FATAL_ERROR
        "Unable to find ${aocl_args_NAME} library. Make sure AOCL 3.0 is installed")
  endif()

  if(WIN32 AND NOT aocl_args_STATIC_LIB)
    find_file(AOCL_${TRGT_NAME}_DLL
      NAMES
        ${CMAKE_SHARED_LIBRARY_PREFIX}${aocl_args_LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
        #lib${aocl_args_LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
      PATHS
        $ENV{AOCL_ROOT}/lib
        $ENV{HOME}/amd/aocl/3.0
        $ENV{HOME}/amd/aocl/3.0-6
      PATH_SUFFIXES
        lib
        bin
      NO_SYSTEM_ENVIRONMENT_PATH
      )
    set_target_properties(AOCL::${aocl_args_NAME}
      PROPERTIES
      IMPORTED_LOCATION "${AOCL_${TRGT_NAME}_DLL}"
      IMPORTED_IMPLIB "${AOCL_${TRGT_NAME}_LINK_LIBRARY}")
    mark_as_advanced(AOCL_${TRGT_NAME}_DLL)
  endif()
endfunction()

if(NOT AOCL_FIND_COMPONENTS)
  set(AOCL_FIND_COMPONENTS "linalg" "fft" "math" "rng")
endif()

message(STATUS "Looking for AOCL with components: ${AOCL_FIND_COMPONENTS}")

if("linalg" IN_LIST AOCL_FIND_COMPONENTS)
  find_aocl_library(NAME blis LIBRARY_NAME blis)
  find_aocl_library(NAME blis_static LIBRARY_NAME blis STATIC_LIB)
  find_aocl_library(NAME blis_mt LIBRARY_NAME blis-mt)
  find_aocl_library(NAME blis_mt_static LIBRARY_NAME blis-mt STATIC_LIB)

  find_aocl_library(NAME flame LIBRARY_NAME flame)
  find_aocl_library(NAME flame_static LIBRARY_NAME flame STATIC_LIB)
  find_aocl_library(NAME lapacke_static LIBRARY_NAME lapacke STATIC_LIB)

  find_aocl_library(NAME scalapack LIBRARY_NAME scalapack)
  find_aocl_library(NAME scalapack_static LIBRARY_NAME scalapack STATIC_LIB)

  find_aocl_library(NAME sparse LIBRARY_NAME aoclsparse)
  find_aocl_library(NAME sparse_static LIBRARY_NAME aoclsparse STATIC_LIB)

  set(AOCL_linalg_FOUND TRUE)
endif()

if("fft" IN_LIST AOCL_FIND_COMPONENTS)
  find_aocl_library(NAME fftw LIBRARY_NAME fftw3)
  find_aocl_library(NAME fftw_static LIBRARY_NAME fftw3 STATIC_LIB)
  find_aocl_library(NAME fftwf LIBRARY_NAME fftw3f)
  find_aocl_library(NAME fftwf_static LIBRARY_NAME fftw3f STATIC_LIB)
  find_aocl_library(NAME fftwl LIBRARY_NAME fftw3l)
  find_aocl_library(NAME fftwl_static LIBRARY_NAME fftw3l STATIC_LIB)
  find_aocl_library(NAME fftwq LIBRARY_NAME fftw3q)
  find_aocl_library(NAME fftwq_static LIBRARY_NAME fftw3q STATIC_LIB)

  find_aocl_library(NAME fftw_mt LIBRARY_NAME fftw3_omp)
  find_aocl_library(NAME fftw_mt_static LIBRARY_NAME fftw3_omp STATIC_LIB)
  find_aocl_library(NAME fftwf_mt LIBRARY_NAME fftw3f_omp)
  find_aocl_library(NAME fftwf_mt_static LIBRARY_NAME fftw3f_omp STATIC_LIB)
  find_aocl_library(NAME fftwl_mt LIBRARY_NAME fftw3l_omp)
  find_aocl_library(NAME fftwl_mt_static LIBRARY_NAME fftw3l_omp STATIC_LIB)
  find_aocl_library(NAME fftwq_mt LIBRARY_NAME fftw3q_omp)
  find_aocl_library(NAME fftwq_mt_static LIBRARY_NAME fftw3q_omp STATIC_LIB)

  find_aocl_library(NAME fftw_cl LIBRARY_NAME fftw3_mpi)
  find_aocl_library(NAME fftw_cl_static LIBRARY_NAME fftw3_mpi STATIC_LIB)
  find_aocl_library(NAME fftwf_cl LIBRARY_NAME fftw3f_mpi)
  find_aocl_library(NAME fftwf_cl_static LIBRARY_NAME fftw3f_mpi STATIC_LIB)
  find_aocl_library(NAME fftwl_cl LIBRARY_NAME fftw3l_omp)
  find_aocl_library(NAME fftwl_cl_static LIBRARY_NAME fftw3l_mpi STATIC_LIB)

  set(AOCL_fft_FOUND TRUE)
endif()

if("math" IN_LIST AOCL_FIND_COMPONENTS)
  find_aocl_library(NAME math LIBRARY_NAME alm)
  find_aocl_library(NAME math_static LIBRARY_NAME alm STATIC_LIB)

  set(AOCL_math_FOUND TRUE)
endif()

if("rng" IN_LIST AOCL_FIND_COMPONENTS)
  find_aocl_library(NAME rng LIBRARY_NAME rng_amd)
  find_aocl_library(NAME rng_static LIBRARY_NAME rng_amd STATIC_LIB)
  find_aocl_library(NAME rng_mt LIBRARY_NAME rng_omp_amd)
  find_aocl_library(NAME rng_mt_static LIBRARY_NAME rng_omp_amd STATIC_LIB)
  find_aocl_library(NAME secure_rng LIBRARY_NAME amdsecrng)
  find_aocl_library(NAME secure_rng_static LIBRARY_NAME amdsecrng STATIC_LIB)

  set(AOCL_rng_FOUND TRUE)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AOCL
  REQUIRED_VARS
    AOCL_INCLUDE_DIR
    ${_aocl_required_vars}
  HANDLE_COMPONENTS
  )

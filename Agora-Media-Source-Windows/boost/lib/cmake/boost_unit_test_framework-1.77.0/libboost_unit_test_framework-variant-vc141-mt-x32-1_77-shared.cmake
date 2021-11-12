# Generated by Boost 1.77.0

# address-model=32

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  _BOOST_SKIPPED("boost_unit_test_framework-vc141-mt-x32-1_77.lib" "32 bit, need 64")
  return()
endif()

# layout=versioned

# toolset=vc141

if(Boost_COMPILER)
  if(NOT Boost_COMPILER STREQUAL "vc141" AND NOT Boost_COMPILER STREQUAL "-vc141")
    _BOOST_SKIPPED("boost_unit_test_framework-vc141-mt-x32-1_77.lib" "vc141, Boost_COMPILER=${Boost_COMPILER}")
    return()
  endif()
else()
  if(BOOST_DETECTED_TOOLSET AND NOT BOOST_DETECTED_TOOLSET STREQUAL "vc141")
    _BOOST_SKIPPED("boost_unit_test_framework-vc141-mt-x32-1_77.lib" "vc141, detected ${BOOST_DETECTED_TOOLSET}, set Boost_COMPILER to override")
    return()
  endif()
endif()

# link=shared

if(DEFINED Boost_USE_STATIC_LIBS)
  if(Boost_USE_STATIC_LIBS)
    _BOOST_SKIPPED("boost_unit_test_framework-vc141-mt-x32-1_77.lib" "shared, Boost_USE_STATIC_LIBS=${Boost_USE_STATIC_LIBS}")
    return()
  endif()
else()
  if(WIN32 AND NOT _BOOST_SINGLE_VARIANT)
    _BOOST_SKIPPED("boost_unit_test_framework-vc141-mt-x32-1_77.lib" "shared, default on Windows is static, set Boost_USE_STATIC_LIBS=OFF to override")
    return()
  endif()
endif()

# runtime-link=shared

if(Boost_USE_STATIC_RUNTIME)
  _BOOST_SKIPPED("boost_unit_test_framework-vc141-mt-x32-1_77.lib" "shared runtime, Boost_USE_STATIC_RUNTIME=${Boost_USE_STATIC_RUNTIME}")
  return()
endif()

# runtime-debugging=off

if(Boost_USE_DEBUG_RUNTIME)
  _BOOST_SKIPPED("boost_unit_test_framework-vc141-mt-x32-1_77.lib" "release runtime, Boost_USE_DEBUG_RUNTIME=${Boost_USE_DEBUG_RUNTIME}")
  return()
endif()

# threading=multi

if(DEFINED Boost_USE_MULTITHREADED AND NOT Boost_USE_MULTITHREADED)
  _BOOST_SKIPPED("boost_unit_test_framework-vc141-mt-x32-1_77.lib" "multithreaded, Boost_USE_MULTITHREADED=${Boost_USE_MULTITHREADED}")
  return()
endif()

# variant=release

if(NOT "${Boost_USE_RELEASE_LIBS}" STREQUAL "" AND NOT Boost_USE_RELEASE_LIBS)
  _BOOST_SKIPPED("boost_unit_test_framework-vc141-mt-x32-1_77.lib" "release, Boost_USE_RELEASE_LIBS=${Boost_USE_RELEASE_LIBS}")
  return()
endif()

if(Boost_VERBOSE OR Boost_DEBUG)
  message(STATUS "  [x] boost_unit_test_framework-vc141-mt-x32-1_77.lib")
endif()

# Create imported target Boost::unit_test_framework

if(NOT TARGET Boost::unit_test_framework)
  add_library(Boost::unit_test_framework SHARED IMPORTED)

  set_target_properties(Boost::unit_test_framework PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_BOOST_INCLUDEDIR}"
    INTERFACE_COMPILE_DEFINITIONS "BOOST_ALL_NO_LIB"
  )
endif()

# Target file name: boost_unit_test_framework-vc141-mt-x32-1_77.lib

get_target_property(__boost_imploc Boost::unit_test_framework IMPORTED_IMPLIB_RELEASE)
if(__boost_imploc)
  message(SEND_ERROR "Target Boost::unit_test_framework already has an imported location '${__boost_imploc}', which is being overwritten with '${_BOOST_LIBDIR}/boost_unit_test_framework-vc141-mt-x32-1_77.lib'")
endif()
unset(__boost_imploc)

set_property(TARGET Boost::unit_test_framework APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(Boost::unit_test_framework PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE CXX
  IMPORTED_IMPLIB_RELEASE "${_BOOST_LIBDIR}/boost_unit_test_framework-vc141-mt-x32-1_77.lib"
  )

set_target_properties(Boost::unit_test_framework PROPERTIES
  MAP_IMPORTED_CONFIG_MINSIZEREL Release
  MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
  )

set_property(TARGET Boost::unit_test_framework APPEND
  PROPERTY INTERFACE_COMPILE_DEFINITIONS "BOOST_UNIT_TEST_FRAMEWORK_DYN_LINK"
  )

list(APPEND _BOOST_UNIT_TEST_FRAMEWORK_DEPS headers)

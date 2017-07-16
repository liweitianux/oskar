#
# cmake/oskar_build_macros.cmake
#

#
# A collection of cmake macros used by the oskar build system.
#

include(oskar_cmake_utilities)

# Macro to build and install oskar apps.
#
# Usage:
#   oskar_app(name
#         [NO_INSTALL]
#         source1 source2 ...
#         [EXTRA_LIBS lib1 lib2 ...]
#   )
#
# name       = Name of the app (binary name)
# source1..N = List of sources from which to build the app
# EXTRA_LIBS = List of additional libraries to link the app against.
# NO_INSTALL = Do not install this app (i.e. don't add to the make install
#              target).
#
# Note: Named options can appear in any order
#
macro(OSKAR_APP)
    parse_arguments(APP   # prefix
        "EXTRA_LIBS"      # arg names
        "NO_INSTALL"      # option names
        ${ARGN}
    )
    CAR(APP_NAME ${APP_DEFAULT_ARGS})
    CDR(APP_SOURCES ${APP_DEFAULT_ARGS})

    # Create a target name from the app name.
    # Note:
    # - Appending app allows for binaries to exist with the same name
    # as targets used elsewhere.
    # - app targets can then be build individually using the command:
    #      $ make <name>_app
    #   where <name> is the (binary) name of the app to be built.
    set(target "${APP_NAME}_app")

    add_executable(${target} ${APP_SOURCES})
    target_link_libraries(${target}
        oskar_apps          # default libs
        ${APP_EXTRA_LIBS}   # extra libs
    )
    # We do need the OpenMP flags here, otherwise programs will crash.
    if (MSVC)
        set_target_properties(${target} PROPERTIES
            COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
            OUTPUT_NAME   "${APP_NAME}"
        )
    else ()
        set_target_properties(${target} PROPERTIES
            INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${OSKAR_LIB_INSTALL_DIR}
            INSTALL_RPATH_USE_LINK_PATH TRUE
            COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
            LINK_FLAGS    "${OpenMP_CXX_FLAGS}"
            OUTPUT_NAME   "${APP_NAME}"
        )
    endif()

    # Create the install target if the NO_INSTALL option isn't specified.
    if (NOT APP_NO_INSTALL)
        install(TARGETS ${target} DESTINATION ${OSKAR_BIN_INSTALL_DIR})
    endif()

endmacro(OSKAR_APP)

# Wraps an OpenCL kernel source file to stringify it.
macro(OSKAR_WRAP_CL SRC_LIST)
    set(OPENCL_STRINGIFY ${PROJECT_SOURCE_DIR}/cmake/oskar_cl_stringify.cmake)
    foreach (CL_FILE ${ARGN})
        get_filename_component(name_ ${CL_FILE} NAME_WE)
        set(CL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${CL_FILE})
        set(CXX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${name_}_cl_kernel.cpp)
        add_custom_command(OUTPUT ${CXX_FILE}
            COMMAND ${CMAKE_COMMAND}
                -D CL_FILE:FILEPATH=${CL_FILE}
                -D CXX_FILE:FILEPATH=${CXX_FILE}
                -P ${OPENCL_STRINGIFY}
            MAIN_DEPENDENCY ${CL_FILE}
            DEPENDS ${CL_FILE} ${OPENCL_STRINGIFY}
            VERBATIM
        )
        list(APPEND ${SRC_LIST} ${CXX_FILE})
    endforeach()
endmacro(OSKAR_WRAP_CL)

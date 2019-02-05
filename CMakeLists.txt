# ------------------------------------------------------------------------------
# Start Cmake syntax
# ------------------------------------------------------------------------------
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# Project's name*** Set PROJECT_NAME variable
project("SimpleNET")


# ------------------------------------------------------------------------------
# Provide default
# ------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("*** Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Use clang if None exists
if(NOT CMAKE_C_COMPILER AND NOT CMAKE_CONFIGURATION_TYPES)
  message("*** Setting C compiler to 'clang' as none was specified.")
  set(CMAKE_C_COMPILER clang CACHE STRING "Choose the compiler." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_C_COMPILER PROPERTY STRINGS "clang" "gcc" "MSVC")
endif()

# Use clang++ if None exists
if(NOT CMAKE_CXX_COMPILER AND NOT CMAKE_CONFIGURATION_TYPES)
  message("*** Setting C++ compiler to 'clang++' as none was specified.")
  set(CMAKE_CXX_COMPILER clang++ CACHE STRING "Choose the compiler." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_CXX_COMPILER PROPERTY STRINGS "clang++" "gcc++" "MSVC")
endif()



# ------------------------------------------------------------------------------
# Create a static or shared library target.
# ------------------------------------------------------------------------------
# Allow the developer to select if Dynamic or Static libraries are built
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
set (LIB_TYPE STATIC)  # Set the LIB_TYPE variable to STATIC
set (MY_LIB_NAME SimpleNET_Static)
if (BUILD_SHARED_LIBS)
    set (LIB_TYPE SHARED)  # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
    set (MY_LIB_NAME SimpleNET_Shared)
endif (BUILD_SHARED_LIBS)
message("*** Library will be build as: ${LIB_TYPE}")


file(
    GLOB
    library_SOURCES
    src/Socket/*
    src/Protocol/*
    src/*
)

add_library(${MY_LIB_NAME} ${LIB_TYPE} ${library_SOURCES})


# See https://cmake.org/pipermail/cmake/2016-May/063400.html
if (BUILD_SHARED_LIBS)
    # Shared library
    target_include_directories(${MY_LIB_NAME}
        PRIVATE BEFORE ${PROJECT_SOURCE_DIR}/include
        PRIVATE
            ${PROJECT_SOURCE_DIR}/extern/Cereal/include
    )

    # Set pre processor command for Shared lib export
    target_compile_definitions(${MY_LIB_NAME}
    PRIVATE
        SIMPLENET_SHARED="USED"
        SIMPLEGL_SHARED_EXPORTS="USED"
    )
else()
    # Static library
    target_include_directories(${MY_LIB_NAME}
        PUBLIC BEFORE ${PROJECT_SOURCE_DIR}/include
        PUBLIC
            ${PROJECT_SOURCE_DIR}/extern/Cereal/include
    )
endif()

set_target_properties(${MY_LIB_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)


# Read only alias used to link created lib
add_library(Net::lib ALIAS ${MY_LIB_NAME})


# ------------------------------------------------------------------------------
# Create a CLIENT link to simpleNET lib
# ------------------------------------------------------------------------------

file(
    GLOB
    client_binary_SOURCES
    ${PROJECT_SOURCE_DIR}/samples/Client/src/*
)

add_executable(
    simpleNET-Client ${client_binary_SOURCES}
)

# See https://cmake.org/pipermail/cmake/2016-May/063400.html
if (BUILD_SHARED_LIBS)
    # Shared lib need to know includes directories
    message("*** Using shared library: SimpleNET headers will be included.")
    target_include_directories(simpleNET-Client
        PRIVATE
            ${PROJECT_SOURCE_DIR}/Client/include
            ${PROJECT_SOURCE_DIR}/extern/Cereal/include
    )

    # Set pre processor command for Shared lib import
    target_compile_definitions(simpleNET-Client
    PRIVATE
        SIMPLENET_SHARED="USED"
    )
else()
    target_include_directories(simpleNET-Client
        PRIVATE
            ${PROJECT_SOURCE_DIR}/samples/Client/include
    )
endif()


set_target_properties(simpleNET-Client
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/samples/Client/bin/${CMAKE_BUILD_TYPE}"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# Using dynamic or static lib based on previous flag (see ${MY_LIB_NAME})
target_link_libraries(simpleNET-Client
    PRIVATE Net::lib
)





# ------------------------------------------------------------------------------
# Create a SERVER link to simpleNET lib
# ------------------------------------------------------------------------------

file(
    GLOB
    server_binary_SOURCES
    ${PROJECT_SOURCE_DIR}/samples/Server/src/*
)

add_executable(
    simpleNET-Server ${server_binary_SOURCES}
)

# See https://cmake.org/pipermail/cmake/2016-May/063400.html
if (BUILD_SHARED_LIBS)
    # Shared lib need to know includes directories
    message("*** Using shared library: SimpleNET headers will be included.")
    target_include_directories(simpleNET-Server
        PRIVATE
            ${PROJECT_SOURCE_DIR}/Server/include
            ${PROJECT_SOURCE_DIR}/extern/Cereal/include
    )

    # Set pre processor command for Shared lib import
    target_compile_definitions(simpleNET-Server
    PRIVATE
        SIMPLENET_SHARED="USED"
    )
else()
    target_include_directories(simpleNET-Server
        PRIVATE
            ${PROJECT_SOURCE_DIR}/samples/Server/include
    )
endif()


set_target_properties(simpleNET-Server
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/samples/Server/bin/${CMAKE_BUILD_TYPE}"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# Using dynamic or static lib based on previous flag (see ${MY_LIB_NAME})
target_link_libraries(simpleNET-Server
    PRIVATE Net::lib
)





# ------------------------------------------------------------------------------
# Only used to debug flags
# ------------------------------------------------------------------------------

# # Informations (Debug)
# message("Your C++ compiler supports these C++ features:")
# foreach(i ${CMAKE_CXX_COMPILE_FEATURES})
#     message("${i}")
# endforeach()

# message("Your CMake version supports these C++ features:")
# get_property(known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
# foreach(i ${known_features})
#     message("${i}")
# endforeach()

# # Get all flags
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()



set(PROJECT_NAME "BlockEditor")
project(${PROJECT_NAME} C CXX)
cmake_minimum_required(VERSION 2.8)


# --------------
# Build settings
# --------------

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH} )

# Install destinations
set( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/_install CACHE PATH "Installation Destination" FORCE)
set( INSTALL_DES ./ CACHE PATH "Where to install to." )

# Warnings for Debug mode
option( USE_STRICT_COMPILE_WARNINGS "Use strict compilation warnings in debug mode." ON )
if( USE_STRICT_COMPILE_WARNINGS )
    if( CMAKE_COMPILER_IS_GNUCXX )
        set( CMAKE_CXX_FLAGS_DEBUG "-Wall" )
        #set( CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Werror" )
        #set( CMAKE_CXX_FLAGS_DEBUG "-pedantic -Wall -Wextra -Wwrite-strings -Winit-self -Wcast-align -Wcast-qual -Wpointer-arith -Wstrict-aliasing -Wformat=2 -Wmissing-declarations -Wmissing-include-dirs -Wuninitialized -Werror-implicit-function-declaration -Wfloat-equal -Wundef -Wstrict-overflow=5 -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Wno-unused-parameter" )
        # use to suppress unused stuff: -Wno-unused-parameter -Wno-unused-result
    endif()
endif()

# Debug is default
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE ) #MinSizeRel

endif()
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE ) #MinSizeRel
message("CMAKE_BUILD_TYPE ::" ${CMAKE_BUILD_TYPE})

add_definitions(
    -fPIC
    -std=c++11
)


# --------------
# Build settings - COTIRE module
# --------------
include(cotire)
if(COMMAND cotire)
    if( NOT COTIRE_MAXIMUM_NUMBER_OF_UNITY_INCLUDES )
        include( ProcessorCount )
        ProcessorCount( N )
        if( NOT N EQUAL 0 )
            set( COTIRE_MAXIMUM_NUMBER_OF_UNITY_INCLUDES "-j${N}" )
        endif()
    endif()
    set( COTIRE_VERBOSE FALSE )
    set( COTIRE_DEBUG FALSE )
endif()


# ------------
# Dependencies
# ------------

### Qt5
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
#set(QT_USE_QTOPENGL 1)

### Lua State
include_directories("libs/LuaState/include")

### Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

### LuaJIT
find_package(LuaJIT REQUIRED)
include_directories(${LUA_INCLUDE_DIR})


# ------------
# Source files
# ------------

file(GLOB SRC_DET "src/*.cpp")
file(GLOB SRC_ITEM "src/item/*.cpp")
file(GLOB SRC_ITEMSTATE "src/item/state/*.cpp")
file(GLOB SRC_SCENE "src/scene/*.cpp")
file(GLOB SRC_COMMAND "src/scene/command/*.cpp")
file(GLOB SRC_STYLE "src/style/*.cpp")
set(SRC ${SRC_DET} ${SRC_ITEM} ${SRC_ITEMSTATE} ${SRC_SCENE} ${SRC_COMMAND} ${SRC_STYLE})

file(GLOB INCL_DET "include/*.h")
file(GLOB INCL_ITEM "include/item/*.h")
file(GLOB INCL_ITEMSTATE "include/item/state/*.h")
file(GLOB INCL_SCENE "include/scene/*.h")
file(GLOB INCL_COMMAND "include/scene/command/*.h")
file(GLOB INCL_STYLE "include/style/*.h")
set(INCL ${INCL_DET} ${INCL_ITEM} ${INCL_ITEMSTATE} ${INCL_SCENE} ${INCL_COMMAND} ${INCL_STYLE})

file(GLOB_RECURSE SCRIPT "scripts/*")
set_source_files_properties(${SCRIPT} PROPERTIES HEADER_FILE_ONLY TRUE)

message("SRC ::" ${SRC})
message("INCL ::" ${INCL})

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
                    )


#aux_source_directory(. SOURCE_FILES)


# --------------
# Build and link
# --------------

message("add_executable of ::" ${PROJECT_NAME})

add_executable( ${PROJECT_NAME}  ${INCL} ${SRC} ${SCRIPT} )

target_link_libraries( ${PROJECT_NAME}
                        Qt5::Widgets
                        ${Boost_LIBRARIES}
                        ${LUA_LIBRARY}
                        )

# 'cotire' the executable
if(COMMAND cotire)
    # this needs CMake 2.8.11 and greater
    if(${CMAKE_VERSION} VERSION_GREATER "2.8.10")
        set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_UNITY_LINK_LIBRARIES_INIT "COPY")
    endif()
    cotire( ${PROJECT_NAME} )

    if(${CMAKE_VERSION} VERSION_LESS "2.8.11")
        target_link_libraries(  ${PROJECT_NAME}_unity
                                Qt5::Widgets
                                ${Boost_LIBRARIES}
                                ${LUA_LIBRARY}
                                )
    endif()

    if( MSVC )
        ### Fix virtual memory range
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm200" CACHE STRING "" FORCE)
    endif()
endif()


# -------
# Install
# -------
install(DIRECTORY scripts/ DESTINATION ${INSTALL_DES}/scripts COMPONENT Data)
install(DIRECTORY examples_scripts/ DESTINATION ${INSTALL_DES}/examples_scripts COMPONENT Data)


install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${INSTALL_DES}
        COMPONENT Runtime )

if(COMMAND cotire)
    add_custom_target(  install_unity
                        COMMAND ${CMAKE_COMMAND} -P "cmake_install.cmake")
    add_dependencies(install_unity ${PROJECT_NAME}_unity)
endif()











